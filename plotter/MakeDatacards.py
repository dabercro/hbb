#! /usr/bin/env python

import os
import sys
import sqlite3

from CrombieTools.LoadConfig import cuts

output = 'datacards/yields'

expr = 'event_class'

if __name__ == '__main__':
    sql_output = output + '.db'

    if not os.path.exists(sql_output) or 'dump' in sys.argv:

        from CrombieTools.AnalysisTools.YieldDump import dumper, SetupFromEnv

        SetupFromEnv()

        dumper.AddDataFile('MET.root')
        dumper.SetDefaultExpr(expr)

        out_dir = os.path.dirname(output)
        if out_dir and not os.path.exists(out_dir):
            os.makedirs(out_dir)

        if 'dump' in sys.argv and os.path.exists(sql_output):
            os.remove(sql_output)

        for region in ['signal', 'tt', 'lightz', 'heavyz']:
            dumper.AddRegion(region, cuts.cut('ZvvHbb', region),
                             cuts.dataMCCuts(region, True),
                             cuts.dataMCCuts(region, False))

        dumper.DumpYieldFiles(sql_output, 10, -1.0, 1.0)

    txt_output = output + '.txt'

    conn = sqlite3.connect(sql_output)
    curs = conn.cursor()

    with open(txt_output, 'w') as datacard:
        write = lambda x: datacard.write(x.strip() + '\n')

        # Define number of channels and stuff
        write("""imax   *   number of channels
jmax   *   number of backgrounds
kmax   *   number of systematics (automatic)""")

        # Write down shape locations
        write('-' * 30)
        write('shape * * datacards/{0}_dat.root {0}__$PROCESS____$CHANNEL {0}__$PROCESS____$CHANNEL__$SYSTEMATIC'.format(expr))

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

        # Placeholder uncertainties
        curs.execute('SELECT COUNT(*) FROM yields WHERE type != "data";')
        tot_bins = curs.fetchone()[0]
        curs.execute('SELECT process, region, source, shape, value FROM uncertainties;')

        for uncproc, uncregion, source, shape, value in curs.fetchall():
            unc_line = name_unc.format(source) + shape_unc.format(shape)
            for proc, region, content in backgrounds:
                unc_val = value if (proc == uncproc or uncproc == '*') and (region == uncregion or uncregion == '*') else '-'
                unc_line += content_fmt.format(unc_val)
            write(unc_line)

        # Systematics
        write('-' * 30)
        for sys, suffs in cuts.sys:
            for suff in suffs:
                write('%s%s param 0.0 1' % (sys, suff))

    conn.close()
