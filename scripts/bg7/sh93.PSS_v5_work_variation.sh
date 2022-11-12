#!/usr/local/bin/zsh

data_dir="/scratch/zpeng/data"
num_t=32
#app_tag="index_adaptive_merge_L_master"
#bin="python ../scripts/test77.PSS_v5_find_L_index_adaptive_merge_L_master.py ./PSS_v5_find_L_index_adaptive_merge_L_master_LG"
app_tag="worker_variation"
#bin="python ../scripts/test77.PSS_v5_find_L_index_adaptive_merge_L_master.py ./PSS_v5_find_L_index_adaptive_merge_L_master_LG"

set -x
###################
#data="deep100m"
#L_master=100
#L_local_lower=10
#L_local_upper=110
#
#index_ratio="1.0"
#P_target="0.9 0.99 0.995 0.997"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}
#
#index_ratio="0.9"
#P_target="0.9 0.99 0.995"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}
#
#index_ratio="0.8"
#P_target="0.9 0.99 0.995"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}
#
#index_ratio="0.7"
#P_target="0.9 0.99"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}
#
##for index_ratio in 0.9 0.8 0.7 0.6 0.5; do
###for index_ratio in 1.1 1.0 0.9 0.8 0.7 0.6 0.5; do
##    tag="${app_tag}.T${num_t}.XR${index_ratio}"
##    eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}
##done
#
##done


##################
#base_dir="sift1m"
#data="sift"
#L=100
#X=999999999
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=100
#X=17
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=100
#X=8
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}


###################
base_dir="deep1b"
data="deep100M"
#L=104
#X=999999999
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=104
#X=112
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=104
#X=56
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}

L=104
#for X in 112; do
for X in 999999999 112 56 28 14 7 3; do
    output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"

    ./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
done


###################
#base_dir="sift1b"
#data="sift100M"
##L=100
##X=999999999
##output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
##
##./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
##
##L=100
##X=39
##output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
##
##./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
##
##L=100
##X=19
##output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
##
##./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=100
#for X in 9 4 2; do
#    output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#    ./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#done




set +x


