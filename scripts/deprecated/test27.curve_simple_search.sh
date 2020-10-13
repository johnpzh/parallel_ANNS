#! /usr/bin/bash
####! /usr/local/bin/zsh
####! /bin/bash

if [ ! $# -eq 3 ]; then
    echo "Usage: $0 <data_directory> <num_threads> <tag>"
    exit
fi
data_dir=$1
num_t=$2
tag=$3

#export KMP_AFFINITY="granularity=fine,compact,0,0"
export KMP_AFFINITY="granularity=fine,compact,1,0"
cd ../cmake-build-release || exit
#####################
## Parallel Single Query Top-M Search
#####################
bin="./profile_seq_single_query_simple_search"
## SIFT
data_path=${data_dir}/sift1m
data_name=sift
echo "----${data_name}----"

#for sub_iter in 1 2; do
#for compt in 50000000 60000000 70000000 80000000 90000000; do
#    label="${tag}.intr${sub_iter}.cmpt$((compt / 1000000))M"
    label="${tag}"
    :> output.${label}.raw.txt
    k=100
    l_start=$k
    l_bound=400
    l_step=30
    for (( l = l_start; l < l_bound; l += l_step )); do
        ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${data_path}/${data_name}.true-100_NN.q-10000.binary $((l/3)) | tee -a output.${label}.raw.txt
    done

    l_start=$l_bound
    l_bound=2000
    l_step=200
    for (( l = l_start; l < l_bound; l += l_step )); do
        ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${data_path}/${data_name}.true-100_NN.q-10000.binary $((l/3)) | tee -a output.${label}.raw.txt
    done

    for l in 2000 4000 8000; do
        ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${data_path}/${data_name}.true-100_NN.q-10000.binary $((l/3)) | tee -a output.${label}.raw.txt
    done

    python3 ../scripts/output_format.py output.${label}.raw.txt output.${label}.row.txt 1 9 7;
    python3 ../scripts/output_row_minimum.py output.${label}.row.txt output.${label}.table.txt 2 0;

#done
#done


##### Middle-4
#bin=./profile_para_single_query_top_m_search_middle_m
#value_M_middle=4
### SIFT
#label="${tag}.intr${sub_iter}.cmpt$((compt / 1000000))M"
#:> output.${label}.raw.txt
#value_m=128
#data_path=${data_dir}/sift1m
#data_name=sift
#echo "----${data_name}----"
#k=100
#l_start=$k
#l_bound=400
#l_step=30
#for (( l = l_start; l < l_bound; l += l_step )); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle} | tee -a output.${label}.raw.txt
#done
#
#l_start=$l_bound
#l_bound=2000
#l_step=200
#for (( l = l_start; l < l_bound; l += l_step )); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle} | tee -a output.${label}.raw.txt
#done
#
#for l in 2000 4000 8000; do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle} | tee -a output.${label}.raw.txt
#done
#
#python3 ../scripts/output_format.py output.${label}.raw.txt output.${label}.row.txt 3 11 9;
#python3 ../scripts/output_row_minimum.py output.${label}.row.txt output.${label}.table.txt 2 0;