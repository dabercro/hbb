#! /usr/bin/env python

import os
import sys
import sqlite3
import datetime

import ROOT

from CrombieTools.LoadConfig import cuts
from CrombieTools.ConfigTools import TreeList

output = 'datacards/yields_%s' % datetime.date.today().strftime('%y%m%d')

expr = 'event_class'

if __name__ == '__main__':
    sql_output = output + '.db'

    fresh = 'dump' in sys.argv or not os.path.exists(sql_output)

    if 'dump' in sys.argv and os.path.exists(sql_output):
        os.remove(sql_output)

    conn = sqlite3.connect(sql_output)
    curs = conn.cursor()

    if fresh:

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


        alltrees = {'data': ['data_obs'],
                    'background': TreeList('MCConfig.txt'),
                    'signal': TreeList('SignalConfig.txt')
                    }

        out_dir = os.path.dirname(output)
        if out_dir and not os.path.exists(out_dir):
            os.makedirs(out_dir)

        for region in ['signal', 'tt', 'lightz', 'heavyz']:
            hist_file = ROOT.TFile('datacards/plots/plots_dat.root')
            for sample_type in alltrees:
                for proc in alltrees[sample_type]:
                    hist = getattr(hist_file, '{expr}__{proc}____{reg}'.format(expr=expr, proc=proc, reg=region))
                    for i in xrange(hist.GetNbinsX()):
                        bin = i + 1
                        curs.execute('INSERT INTO yields VALUES (?, ?, ?, ?, ?, ?)',
                                     (region, proc, bin,
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
        write('shapes * * datacards/plots/plots_dat.root {0}__$PROCESS____$CHANNEL {0}__$PROCESS____$CHANNEL__$SYSTEMATIC'.format(expr))

        # Write down data observations
        write('-' * 30)
        start_fmt = '{:<25}'
        name_unc = '{:<15}'
        shape_unc = '{:<10}'
        info_fmt = '{:<12}   '
        content_fmt = '{:<12.4}   '

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
        curs.execute('SELECT process, region, SUM(contents) FROM yields WHERE type = "signal" GROUP BY region, process ORDER BY process, region;')
        backgrounds = list(curs.fetchall())

        # Then background
        curs.execute('SELECT process, region, SUM(contents) FROM yields WHERE type = "background" GROUP BY region, process ORDER BY process, region;')
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

        # Flat uncertainties

        uncertainties = {
            'lumi': {
                'val': 1.025
                },
            'pileup': {
                'val': 1.05
                },
            'PDF': {
                'val': 1.3
                },
            'diboson': {
                'val': 1.3,
                'procs': ['vv']
                },
            'singletop': {
                'val': 1.3,
                'procs': ['st']
                }
            }
        
        for source, val in uncertainties.iteritems():
            unc_line = name_unc.format(source) + shape_unc.format(val.get('shape', 'lnN'))
            for proc, region, _ in backgrounds:
                unc_val = val['val'] if \
                    (not val.get('procs') or proc in val.get('procs', [])) and \
                    (not val.get('regions') or region in val.get('regions', [])) \
                    else '-'
                unc_line += content_fmt.format(unc_val)
            write(unc_line)

        # Systematics
        write('-' * 30)
        for syst, suffs in cuts.syst:
            for suff in suffs:
                write('%s%s param 0.0 1' % (syst, suff))

    conn.close()
