#! /usr/local/bin/zsh
####! /bin/bash

if [ ! $# -eq 6 ]; then
    echo "Usage: $0 <script> <location_runtime> <location_computation> <location_recall> <location_Queries_per_Second> <repeats>"
    exit
fi

app=$1
loc_rntm=$2
loc_comp=$3
loc_prec=$4
loc_qps=$5
repeats=$6

cd ../cmake-build-release || exit

# Get raw results
${app} | tee output.raw.txt

# Get runtime, computation count, and recalls.
#avg_app=../scripts/output_average.py
../scripts/output_minimum.py output.raw.txt output.runtime.txt ${repeats} ${loc_rntm}
../scripts/output_minimum.py output.raw.txt output.computation.txt ${repeats} ${loc_comp}
../scripts/output_maximum.py output.raw.txt output.recall.txt ${repeats} ${loc_prec}
../scripts/output_maximum.py output.raw.txt output.queries_per_second.txt ${repeats} ${loc_qps}

# Backup
app_name=$(basename ${app} .sh)
cp output.raw.txt output.${app_name}.raw.txt
cp output.runtime.txt output.${app_name}.runtime.txt
cp output.computation.txt output.${app_name}.computation.txt
cp output.recall.txt output.${app_name}.recall.txt
cp output.queries_per_second.txt output.${app_name}.queries_per_second.txt