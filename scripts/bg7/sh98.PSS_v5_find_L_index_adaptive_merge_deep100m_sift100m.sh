#!/usr/local/bin/zsh

data_dir="/scratch/zpeng/data"
#data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
app_tag="index_adaptive_merge"
num_t=32
bin="python ../scripts/test76.PSS_v5_find_L_index_adaptive_merge.py ./PSS_v5_find_L_index_adaptive_merge_LG"

set -x
##################
data="deep100m"
index_ratio="0.9"
L_lower=90
L_upper=800
#L_upper=110

P_target="0.995 0.996 0.997 0.998 0.999"
tag="${app_tag}.T${num_t}.XR${index_ratio}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_lower} ${L_upper} ${index_ratio} ${P_target}
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


