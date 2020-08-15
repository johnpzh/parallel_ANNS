#! /usr/bin/bash
####! /usr/local/bin/zsh
####! /bin/bash

if [ ! $# -eq 2 ]; then
    echo "Usage: $0 <data_directory> <num_threads>"
    exit
fi
data_dir=$1
num_t=$2

export KMP_AFFINITY="granularity=fine,compact,0,0"
#export KMP_AFFINITY="granularity=fine,compact,1,0"
cd ../cmake-build-release || exit
#####################
## Parallel Single Query Top-M Search
#####################
bin="numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge"
#bin=./profile_para_single_query_top_m_search_middle_m
#bin=./profile_para_single_query_top_m_search_better_merge
#value_m=128
value_M_middle=4

## SIFT
value_m=128
data_path=${data_dir}/sift1m
#data_path=/scratch/zpeng/sift1m
data_name=sift
echo "----${data_name}----"
local_L=70
sub_iter=
k=100
l_start=$k
l_bound=400
l_step=30
for (( l = l_start; l < l_bound; l += l_step )); do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle}
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t}
done

l_start=$l_bound
l_bound=2000
l_step=200
for (( l = l_start; l < l_bound; l += l_step )); do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle}
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t}
done

for l in 2000 4000 8000; do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle}
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t}
done

### GIST
#value_m=256
#data_path=${data_dir}/gist1m
##data_path=/scratch/zpeng/gist1m
#data_name=gist
#echo "----${data_name}----"
#k=100
#l_start=$k
#l_bound=400
#l_step=30
#for (( l = l_start; l < l_bound; l += l_step )); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary ${num_t} ${value_M_middle}
##    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary ${num_t}
#done
#
#l_start=$l_bound
#l_bound=2000
#l_step=200
#for (( l = l_start; l < l_bound; l += l_step )); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary ${num_t} ${value_M_middle}
##    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary ${num_t}
#done
#
#for l in 2000 4000 8000; do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary ${num_t} ${value_M_middle}
##    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary ${num_t}
#done
#
### DEEP10M
#value_m=256
#data_path=${data_dir}/deep1b
##data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#echo "----${data_name}----"
#k=100
#l_start=$k
#l_bound=400
#l_step=30
#for (( l = l_start; l < l_bound; l += l_step )); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle}
##    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t}
#done
#
#l_start=$l_bound
#l_bound=2000
#l_step=200
#for (( l = l_start; l < l_bound; l += l_step )); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle}
##    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t}
#done
#
#for l in 2000 4000 8000; do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} ${value_M_middle}
##    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t}
#done



