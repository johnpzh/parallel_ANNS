#! /usr/bin/bash
####! /usr/local/bin/zsh
####! /bin/bash

if [ $# -lt 6 ]; then
    echo "Usage: $0 <script> <tag> <location_query-per-second> <location_accuracy> <repeats> <data_directory> [num_threads]"
    exit
fi

app=$1
tag=$2
loc_qps=$3
loc_accry=$4
repeats=$5
data_dir=$6
num_threads=$7

#cd ../cmake-build-release || exit

# Get raw results
${app} ${data_dir} ${num_threads} | tee output.${tag}.raw.txt

# Get curve results
python3 ../scripts/output_maximum.py output.${tag}.raw.txt output.${tag}.query-per-second.txt ${repeats} ${loc_qps}
python3 ../scripts/output_maximum.py output.${tag}.raw.txt output.${tag}.accuracy.txt ${repeats} ${loc_accry}
python3 ../scripts/combine_two_cols.py output.${tag}.query-per-second.txt output.${tag}.accuracy.txt output.${tag}.curve.txt

## Backup
#app_name=$(basename ${app} .sh)
#cp output.${tag}.raw.txt output.${tag}.${app_name}.raw.txt
#cp output.${tag}.query-per-second.txt output.${tag}.${app_name}.query-per-second.txt
#cp output.${tag}.accuracy.txt output.${tag}.${app_name}.accuracy.txt
#cp output.${tag}.curve.txt output.${tag}.${app_name}.curve.txt
