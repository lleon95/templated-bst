#!/usr/local/bin/gnuplot --persist

set title "{/=15 Finding process profiling} \n\n {/=12 Using bst and std::map" # plot title
set xlabel 'Number of elements randomly generated'                               # x-axis label
set ylabel 'Runtime (s)'                                                         # y-axis label
set logscale xy
set format y "%e"

plot '../reports/report_bst.csv' using 1:10 with linespoints title 'Copy (bst)',   \
'../reports/report_bst.csv' using 1:13 with linespoints title 'Move (bst)',\
'../reports/report_map.csv' using 1:9 with linespoints title 'Copy (std::map)',   \
'../reports/report_map.csv' using 1:12 with linespoints title 'Move (std::map)'

