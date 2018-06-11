#! /usr/bin/env python

import os
import sys
import sqlite3
import datetime

import ROOT

from CrombieTools.LoadConfig import cuts
from CrombieTools.ConfigTools import TreeList

# Flat uncertainties
uncertainties = {
    'lumi': {
        'val': 1.025
        },
    'pileup': {
        'val': 1.05
        },
    'trig_met': {
        'val': 1.03
        },
    'uncl_met': {
        'val': 1.03
        },
#    'diboson': {
#        'val': 1.3,
#        'procs': ['vv']
#        },
#    'singletop': {
#        'val': 1.3,
#        'procs': ['st']
#        },
    'wj': {
        'val': 1.3,
        'shape': 'lnU',
        'procs': ['wj', 'wjb', 'wjbb']
        },
    'wfact': {
        'shape': 'shape',
        'procs': ['wj', 'wjb', 'wjbb']
        },
    'zfact': {
        'shape': 'shape',
        'procs': ['zj', 'zjb', 'zjbb']
        },
    'renorm': {
        'shape': 'shape',
        'procs': []   # Will be filled with proc vetos
        },
#    'ttfatsf': {
#        'val': 2.0,
#        'regions': ['boosted__tt']
#        },
#    'lightzfatsf': {
#        'val': 2.0,
#        'regions': ['boosted__lightz']
#        },
#    'heavyfatsf': {
#        'val': 2.0,
#        'regions': ['boosted__heavyz', 'boosted__signal']
#        }
    }
        
for boson in ['w', 'z']:
    for proc in uncertainties[boson + 'fact']['procs']:
        uncertainties['renorm']['procs'].append('!' + proc)

    uncertainties[boson + 'ren'] = uncertainties[boson + 'fact']

uncertainties['fact'] = uncertainties['renorm']

# These are rateParams
keys = ['tt']

shapes = list(cuts.syst) + list(cuts.env)

#for bos in 'zw':
for bos in 'z':
    for num_b in range(3):
        keys.append(bos + 'j' + 'b'*num_b)

datething = datetime.date.today().strftime('%y%m%d') + ('_%s' % sys.argv[1] if len(sys.argv) > 1 else '')
output = 'datacards/yields_%s' % datething

expr = {
    'inclusive': {
#        'signal': 'boost_class',
        'signal': 'event_class',
        'default': 'cmva_jet2_cmva'
#        'default': 'jet2_deepCSVb'
        }
    }

if 'inclusive' not in sys.argv:
    # Overwrite expr with just inclusive
    expr['boosted'] = {
        'signal': 'event_class',
        'default': 'ak8fatjet1_mSD_corr'
        }
if 'boosted' in sys.argv:
    expr.pop('inclusive')

alltrees = {'data': ['data_obs'],
            'background': TreeList('MCConfig010.txt'),
#            'background': TreeList('MCConfig.txt'),
#            'signal': TreeList('SignalConfig.txt')
            'signal': TreeList('SignalConfig010.txt')
            }

if __name__ == '__main__':
    sql_output = output + '.db'

    if os.path.exists(sql_output):
        os.remove(sql_output)

    conn = sqlite3.connect(sql_output)
    curs = conn.cursor()

    os.system('./merge_plots.sh')

    curs.execute("""
                 CREATE TABLE IF NOT EXISTS yields (
                 region VARCHAR(64),
                 process VARCHAR(64),
                 bin INT,
                 contents DOUBLE,
                 stat_unc DOUBLE,
                 type VARCHAR(32),
                 PRIMARY KEY (region, process, bin)
                 )""")

    out_dir = os.path.dirname(output)
    if out_dir and not os.path.exists(out_dir):
        os.makedirs(out_dir)

    in_file = ROOT.TFile('datacards/plots/plots_dat.root')
    hist_file = ROOT.TFile('datacards/plots/plots_dat.root.%s' % datething, 'RECREATE')
    for category in expr:
        for region in ['signal', 'tt', 'lightz', 'heavyz']:
            channel = '%s__%s' % (category, region)
            for sample_type in alltrees:
                for proc in alltrees[sample_type]:
                    hist = getattr(in_file, '{expr}__{proc}____{reg}'.format(
                            expr=expr[category].get(region, expr[category]['default']),
                            proc=proc, reg=channel))

                    if hist.GetEntries() < 5:
                        continue

                    hist_file.WriteTObject(hist, '{proc}____{reg}'.format(proc=proc, reg=channel))

                    for syst in shapes:
                        for direction in ['Up', 'Down']:
                            outname = '{proc}____{reg}__{syst}{direction}'.format(proc=proc, reg=channel, syst=syst, direction=direction)
                            hist_file.WriteTObject(
                                getattr(in_file, '{expr}__{out}'.format(expr=expr[category].get(region, expr[category]['default']), out=outname)),
                                outname)
                            
                    for i in xrange(hist.GetNbinsX()):
                        bin = i + 1
                        curs.execute('INSERT INTO yields VALUES (?, ?, ?, ?, ?, ?)',
                                     (channel, proc, bin,
                                      hist.GetBinContent(bin), hist.GetBinError(bin),
                                      sample_type))
    conn.commit()

    txt_output = output + '.txt'

    with open(txt_output, 'w') as datacard:
        write = lambda x: datacard.write(x.strip() + '\n')

        # Define number of channels and stuff
        write("""imax   *   number of channels
jmax   *   number of backgrounds
kmax   *   number of systematics (automatic)""")

        # Write down shape locations
        write('-' * 30)
        write('shapes * * {1}/datacards/plots/plots_dat.root.{0} $PROCESS____$CHANNEL $PROCESS____$CHANNEL__$SYSTEMATIC'.format(datething, os.path.abspath('.')))

        # Write down data observations
        write('-' * 30)
        start_fmt = '{:<25}'
        name_unc = '{:<15}'
        shape_unc = '{:<10}'
        info_fmt = '{:<20}   '
        content_fmt = '{:<20.4}   '

        bin_line = start_fmt.format('bin')
        obs_line = start_fmt.format('observation')

        curs.execute('SELECT region, SUM(contents) FROM yields WHERE type = "data" GROUP BY region ORDER BY region;')

        for region, content in curs.fetchall():
            bin_line += info_fmt.format(region)
            obs_line += info_fmt.format(content)

        write(bin_line)
        write(obs_line)

        # Write down MC expectations
        write('-' * 30)
        bin_line = start_fmt.format('bin')
        proc_line = start_fmt.format('process')
        proc_enum = start_fmt.format('process')
        obs_line = start_fmt.format('rate')

        # Start with signal
        proc_count = 0

        simple = lambda s: 'SELECT process, region, SUM(contents) AS sum FROM yields WHERE type = "%s" GROUP BY region, process ORDER BY process, region' % s
        query = lambda s: 'WITH data AS (%s), query AS (%s) SELECT query.process, query.region, query.sum FROM query INNER JOIN data ON query.region = data.region' % \
            (simple('data'), simple(s))

        curs.execute(query('signal'))
        backgrounds = list(curs.fetchall())

        proc_count -= len(set([proc for proc, _, _ in backgrounds])) - 1

        # Then background
        curs.execute(query('background'))
        backgrounds.extend(list(curs.fetchall()))

        prev_proc = backgrounds[0][0]
        for process, region, content in backgrounds:
            if process != prev_proc:
                prev_proc = process
                proc_count += 1
            bin_line += info_fmt.format(region)
            proc_line += info_fmt.format(process)
            proc_enum += info_fmt.format(proc_count)
            obs_line += content_fmt.format(max(content, 0.01))

        write(bin_line)
        write(proc_line)
        write(proc_enum)
        write(obs_line)

        # Uncertainties
        write('-' * 30)

        for source, val in uncertainties.iteritems():
            unc_line = name_unc.format(source) + shape_unc.format(val.get('shape', 'lnN'))
            for proc, region, _ in backgrounds:
                unc_val = val.get('val', '1.0') if \
                    (not val.get('procs') or (proc in val['procs'] or ('!' + proc not in val['procs'] and val['procs'][0].startswith('!')))) and \
                    (not val.get('regions') or region in val.get('regions', [])) \
                    else '-'
                unc_line += content_fmt.format(unc_val)
            write(unc_line)

        # Systematics
        for syst in shapes:
            if syst in uncertainties:
                continue

            unc_line = name_unc.format(syst) + shape_unc.format('shape')
            for _ in backgrounds:
                unc_line += content_fmt.format('1.0')
            write(unc_line)

        write('-' * 30)

        simple = lambda s: 'SELECT DISTINCT(region) FROM yields WHERE type = "%s"' % s
        query = lambda s: 'WITH data AS (%s), back as (%s) SELECT back.region FROM back INNER JOIN data ON data.region = back.region' % \
            (simple('data'), simple(s))

        curs.execute(query('background'))
        regions = list(curs.fetchall())
        for key in keys:
            for region in regions:
                write('SF_{proc}  rateParam  {region}  {proc}  1  [0.2,5]'.format(proc=key, region=region[0]))

        write('* autoMCStats 1 1 1')

    conn.close()
