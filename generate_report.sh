#!/bin/bash

# Make if needed
make
EXE=./main.x
mkdir reports

NUM_ITERS=1000
echo "elements,insertion_copy,insertion_move,emplace,clear,find_unbalanced,find_balanced,suscription_retrieve,subscription_setting,copy_assignment,copy_construction,move_assignment,move_construction" > reports/report_bst.csv
echo "elements,insertion_copy,insertion_move,emplace,clear,find_unbalanced,suscription_retrieve,subscription_setting,copy_assignment,copy_construction,move_assignment,move_construction" > reports/report_map.csv

# Run for several tree sizes
for i in 10 100 1000 10000 100000;
do
  # Run the script and get results
  $EXE $i $NUM_ITERS $NUM_ITERS > reports/report_${i}.txt
  # Results from lines 17 to 27 -> bst
  RESULT=$(awk "NR==16, NR==27 {print \$5} END {}" ORS="" reports/report_${i}.txt)
  echo "$i,$RESULT" >> reports/report_bst.csv
  # Results from lines 43 to 53 -> std::map
  RESULT=$(awk "NR==43, NR==53 {print \$5} END {}" ORS="" reports/report_${i}.txt)
  echo "$i,$RESULT" >> reports/report_map.csv
  rm reports/report_${i}.txt
done
