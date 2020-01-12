#!/usr/local/bin/gnuplot --persist

set title "{/=15 Lookups profiling} \n\n {/=12 Using bst and std::map" # plot title
set xlabel 'Number of elements randomly generated'                               # x-axis label
set ylabel 'Runtime (s)'                                                         # y-axis label
set logscale xy
set format y "%e"
f(x) = 1.0e-07*log(x)

plot '../reports/report_bst.csv' using 1:6 with linespoints title 'Find Unbalanced (bst)',   \
'../reports/report_bst.csv' using 1:7 with linespoints title 'Find Balanced (bst)',   \
'../reports/report_map.csv' using 1:6 with linespoints title 'Find (std::map)', \
f(x) t 'Ideal O(log(n))'

