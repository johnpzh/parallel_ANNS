#! /usr/local/bin/zsh
####! /bin/bash

#set -x
## OpenMP Affinity for ICC
export KMP_AFFINITY="granularity=fine,compact,1,0"
#export KMP_AFFINITY="granularity=core,compact,1,0"
## Above are almost the same for KNL

cd ../cmake-build-release || exit
#bin_panns=./profile_top_m_para_search
#bin_panns=./profile_para_single_query_top_m_search_sync_or_not
#bin_panns=./profile_top_m_seq_search_bit_CAS
#bin_panns=./profile_top_m_seq_search_myths_M
#bin_panns=./profile_para_single_query_top_m_search_by_sort
bin_panns=./profile_para_single_query_top_m_search_better_merge
#bin_panns=./profile_para_single_query_top_m_search_no_merge
num_t_max=8
#num_t_max=1
value_m=128

#
## SIFT
value_m=128
data_path=/data/zpeng/sift1m
#data_path=/scratch/zpeng/sift1m
data_name=sift
k=200
l=200
echo "----${data_name}----"
for ((num_t = 8; num_t <= num_t_max; num_t *= 2)); do
    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t}
done

## GIST
value_m=128
data_path=/data/zpeng/gist1m
#data_path=/scratch/zpeng/gist1m
data_name=gist
k=400
l=400
echo "----${data_name}----"
for ((num_t = 8; num_t <= num_t_max; num_t *= 2)); do
    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary ${num_t}
done

## DEEP10M
value_m=128
data_path=/data/zpeng/deep1b
#data_path=/scratch/zpeng/deep1b
data_name=deep10M
k=400
l=400
echo "----${data_name}----"
for ((num_t = 8; num_t <= num_t_max; num_t *= 2)); do
    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t}
done
