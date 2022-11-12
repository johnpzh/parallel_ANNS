#!/usr/local/bin/zsh

#data_dir="/scratch/zpeng/data"
data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
app_tag="PSS_v5_adaptive_merge_mem_bdw"
num_t=32
bin="pcm-memory.x -- python3 ../scripts/test77.PSS_v5_find_L_index_adaptive_merge_L_master.py ./pcm_mem_PSS_v5_find_L_index_adaptive_merge_L_master_LG"
#bin="python3 ../scripts/test77.PSS_v5_find_L_index_adaptive_merge_L_master.py ./pcm_mem_PSS_v5_find_L_index_adaptive_merge_L_master_LG"

output="output_${app_tag}.txt"
:> ${output}

set -x

##################
data="deep100m"
L_master=100
L_local_lower=5
L_local_upper=233
#L_local_upper=400
#L_local_upper=120

#index_ratio="1.0"
#P_target="0.9 0.99 0.995 0.997"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}

#for index_ratio in 0.1 0.2 0.3 0.4; do
#for index_ratio in 0.5 0.6 0.7 0.8; do
index_ratio="0.9"
#P_target="0.999"
#P_target="0.996 0.997 0.998 0.999"
P_target="0.9 0.91 0.92 0.93 0.94 \
          0.95 0.96 0.97 0.98 0.99 \
          0.991 0.992 0.993 0.994 0.995 \
          0.996 0.997 0.998 0.999"
tag="${app_tag}.T${num_t}.XR${index_ratio}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target} | tee -a ${output}
#done

#index_ratio="0.8"
#P_target="0.9 0.99 0.995"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}
#
#index_ratio="0.7"
#P_target="0.9 0.99"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}

#index_ratio="0.6"
#P_target="0.9 0.99"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}

#index_ratio="0.5"
#P_target="0.9 0.99"
#tag="${app_tag}.T${num_t}.XR${index_ratio}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}

###################
#data="deep100m"
#L_lower=90
#L_upper=800
##L_upper=110
#
#P_target="0.9 0.99 0.995 0.997 0.999"
##P_target="0.999 0.997 0.995 0.99 0.9"
#
##for index_ratio in 1.1 1.0 0.6 0.5; do
##for index_ratio in 0.9 0.8 0.7 0.6 0.5; do
#for index_ratio in 1.1 1.0 0.9 0.8 0.7 0.6 0.5; do
#    tag="${app_tag}.T${num_t}.XR${index_ratio}"
#    eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_lower} ${L_upper} ${index_ratio} ${P_target}
#done
#
###################
#data="sift100m"
#L_lower=90
#L_upper=400
##L_upper=110
#
#P_target="0.9 0.99 0.995 0.997 0.999"
#
##for index_ratio in 1.1 1.0 0.6 0.5; do
#for index_ratio in 1.1 1.0 0.9 0.8 0.7 0.6 0.5; do
##for index_ratio in 1.1 1.0 0.9 0.8 0.7 0.6 0.5 0.4 0.3 0.2 0.1; do
#    tag="${app_tag}.T${num_t}.XR${index_ratio}"
#    eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_lower} ${L_upper} ${index_ratio} ${P_target}
#done

###################
### test
#data="deep100m"
#L_lower=90
#L_upper=800
##L_upper=110
#
#P_target="0.9 0.99 0.995 0.997 0.999"
#
#for index_ratio in 0.5; do
#    tag="nolimit.${app_tag}.T${num_t}.XR${index_ratio}"
#    eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_lower} ${L_upper} ${index_ratio} ${P_target}
#done

set +x


