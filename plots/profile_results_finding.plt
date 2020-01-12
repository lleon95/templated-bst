#!/usr/local/bin/gnuplot --persist

set title "{/=15 Finding process profiling} \n\n {/=12 Using bst and std::map" # plot title
set xlabel 'Number of elements randomly generated'                               # x-axis label
set ylabel 'Runtime (s)'                                                         # y-axis label
set logscale xy
set format y "%e"

plot '../reports/report_bst.csv' using 1:6 with linespoints title 'Find Unbalanced (bst)',   \
'../reports/report_bst.csv' using 1:7 with linespoints title 'Find Balanced (bst)',   \
'../reports/report_bst.csv' using 1:8 with linespoints title 'Subscription (bst)',\
'../reports/report_map.csv' using 1:6 with linespoints title 'Find (std::map)',   \
'../reports/report_map.csv' using 1:7 with linespoints title 'Subscription (std::map)'

