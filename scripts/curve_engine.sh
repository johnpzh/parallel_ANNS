#! /usr/local/bin/zsh
####! /bin/bash

if [ $# -lt 4 ]; then
    echo "Usage: $0 <script> <location_query-per-second> <location_accuracy> <repeats> [num_threads]"
    exit
fi

app=$1
loc_qps=$2
loc_accry=$3
repeats=$4
num_threads=$5

cd ../cmake-build-release || exit

# Get raw results
${app} ${num_threads} | tee output.raw.txt

# Get curve results
../scripts/output_maximum.py output.raw.txt output.query-per-second.txt ${repeats} ${loc_qps}
../scripts/output_maximum.py output.raw.txt output.accuracy.txt ${repeats} ${loc_accry}
../scripts/combine_two_cols.py output.query-per-second.txt output.accuracy.txt output.curve.txt

# Backup
app_name=$(basename ${app} .sh)
cp output.raw.txt output.${app_name}.raw.txt
cp output.query-per-second.txt output.${app_name}.query-per-second.txt
cp output.accuracy.txt output.${app_name}.accuracy.txt
cp output.curve.txt output.${app_name}.curve.txt
