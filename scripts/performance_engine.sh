#! /usr/bin/bash
####! /usr/local/bin/zsh
####! /bin/bash

if [ $# -ne 9 ]; then
    echo "Usage: $0 <script> <tag> <location_runtime> <location_computation> <location_recall> <location_Queries_per_Second> <location_merge_time> <repeats> <data_directory>"
    exit
fi

app=$1
tag=$2
loc_rntm=$3
loc_comp=$4
loc_prec=$5
loc_qps=$6
loc_merge=$7
repeats=$8
data_dir=$9

cd ../cmake-build-release || exit

# Get raw results
${app} ${data_dir} | tee output.${tag}.raw.txt

# Get runtime, computation count, and recalls.
#avg_app=../scripts/output_average.py
python3 ../scripts/output_minimum.py output.${tag}.raw.txt output.${tag}.runtime.txt ${repeats} ${loc_rntm}
python3 ../scripts/output_minimum.py output.${tag}.raw.txt output.${tag}.computation.txt ${repeats} ${loc_comp}
python3 ../scripts/output_maximum.py output.${tag}.raw.txt output.${tag}.recall.txt ${repeats} ${loc_prec}
python3 ../scripts/output_maximum.py output.${tag}.raw.txt output.${tag}.queries_per_second.txt ${repeats} ${loc_qps}
python3 ../scripts/output_minimum.py output.${tag}.raw.txt output.${tag}.merge_time.txt ${repeats} ${loc_merge}

python3 ../scripts/combine_four_cols.py output.${tag}.runtime.txt output.${tag}.recall.txt output.${tag}.computation.txt output.${tag}.merge_time.txt output.${tag}.performance_table.txt

## Backup
#app_name=$(basename ${app} .sh)
#cp output.${tag}.raw.txt output.${tag}.${app_name}.raw.txt
#cp output.${tag}.runtime.txt output.${tag}.${app_name}.runtime.txt
#cp output.${tag}.computation.txt output.${tag}.${app_name}.computation.txt
#cp output.${tag}.recall.txt output.${tag}.${app_name}.recall.txt
#cp output.${tag}.queries_per_second.txt output.${tag}.${app_name}.queries_per_second.txt