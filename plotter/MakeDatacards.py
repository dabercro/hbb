#! /usr/bin/env python

import os
import sys
import sqlite3

output = 'datacards/yields'

if __name__ == '__main__':
    sql_output = output + '.db'

    if not os.path.exists(sql_output) or 'dump' in sys.argv:

        from CrombieTools.AnalysisTools.YieldDump import dumper, SetupFromEnv
        from CrombieTools.LoadConfig import cuts

        dumper.SetAllHistName('')

        SetupFromEnv()

        dumper.AddDataFile('MET.root')
        dumper.SetDefaultExpr('hbbm')

        out_dir = os.path.dirname(output)
        if out_dir and not os.path.exists(out_dir):
            os.makedirs(out_dir)

        if 'dump' in sys.argv and os.path.exists(sql_output):
            os.remove(sql_output)

        for region in ['signal', 'scaledtt', 'lightz', 'heavyz', 'multijet']:
            dumper.AddRegion(region, cuts.cut('ZvvHbb', region),
                             cuts.dataMCCuts(region, True),
                             cuts.dataMCCuts(region, False))

        dumper.DumpYieldFiles(sql_output, 5, 0, 200)
        os.system('crombie setupdatacard links.txt ' + sql_output)


    txt_output = output + '.txt'

    conn = sqlite3.connect(sql_output)
    curs = conn.cursor()

    with open(txt_output, 'w') as datacard:
        write = lambda x: datacard.write(x.strip() + '\n')

        curs.execute('SELECT COUNT(DISTINCT(process)) FROM yields WHERE type = "background";')

        # Defin number of channels and stuff
        write("""imax   1   number of channels
jmax   %i   number of backgrounds
kmax   *   number of systematics (automatic)""" % curs.fetchone())

        start_fmt = '{:<25}'
        name_unc = '{:<15}'
        shape_unc = '{:<10}'
        info_fmt = '{:<12}   '
        content_fmt = '{:<12.4}   '

        # Write down data observations
        write('-' * 30)
        bin_line = start_fmt.format('bin')
        obs_line = start_fmt.format('observation')

        curs.execute('SELECT bin, region, SUM(contents) FROM yields WHERE type = "data" GROUP BY bin, region ORDER BY region, bin;')

        for bin, region, content in curs.fetchall():
            bin_line += info_fmt.format('%s_%i' % (region, bin))
            obs_line += content_fmt.format(content)

        write(bin_line)
        write(obs_line)

        # Write down MC expectations
        write('-' * 30)
        bin_line = start_fmt.format('bin')
        proc_line = start_fmt.format('proc')
        proc_enum = start_fmt.format('proc')
        obs_line = start_fmt.format('rate')

        # Start with signal
        prev_proc = ''
        proc_count = 0
        curs.execute('SELECT process, bin, region, SUM(contents) FROM yields WHERE type = "signal" GROUP BY bin, region, process ORDER BY process, region, bin;')
        for process, bin, region, content in curs.fetchall():
            prev_proc = process
            bin_line += info_fmt.format('%s_%i' % (region, bin))
            proc_line += info_fmt.format(process)
            proc_enum += info_fmt.format(proc_count)
            obs_line += content_fmt.format(content)

        # Then background
        curs.execute('SELECT process, bin, region, SUM(contents) FROM yields WHERE type = "background" GROUP BY bin, region, process ORDER BY process, region, bin;')
        backgrounds = list(curs.fetchall())
        for process, bin, region, content in backgrounds:
            if process != prev_proc:
                prev_proc = process
                proc_count += 1
            bin_line += info_fmt.format('%s_%i' % (region, bin))
            proc_line += info_fmt.format(process)
            proc_enum += info_fmt.format(proc_count)
            obs_line += content_fmt.format(content)

        write(bin_line)
        write(proc_line)
        write(proc_enum)
        write(obs_line)

        # Uncertainties
        write('-' * 30)

        # Placeholder uncertainties
        curs.execute('SELECT COUNT(*) FROM yields WHERE type != "data";')
        tot_bins = curs.fetchone()[0]
        curs.execute('SELECT uncbin, uncprocess, uncregion, source, shape, value FROM uncertainties;')
        for uncbin, uncproc, uncregion, source, shape, value in curs.fetchall():
            unc_line = name_unc.format(source) + shape_unc.format(shape)
            for proc, bin, region, content in backgrounds:
                unc_val = value if (proc == uncproc or proc == '*') and (region == uncregion or region == '*') and (bin == uncbin or bin == '0') else '-'
                punc_line += content_fmt.format()
            write(unc_line)

    conn.close()
