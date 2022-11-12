#!/usr/local/bin/zsh

#data_dir="/scratch/zpeng/data"
data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
app_tag="PSS_v5_index_adaptive_merge_pcm_mem"
#num_t=64
num_t=32
bin="python ../scripts/test77.PSS_v5_find_L_index_adaptive_merge_L_master.py ./pcm_mem_PSS_v5_find_L_index_adaptive_merge_L_master_LG"
output="output_${app_tag}.txt"
:> ${output}
set -x

P_target=0.999
tag="${app_tag}_T${num_t}_P${P_target}"
index_ratio="0.9"
##################
## DEEP100M
##################
data=deep100m

L_master=100
L_local_lower=193
L_local_upper=194
{
    echo ""
    echo "###############"
    echo "## ${data}"
    echo "###############"
} >> ${output}

#for index_ratio in 0.5 0.6 0.7 0.8; do
#index_ratio="0.9"

tag="${app_tag}.T${num_t}.XR${index_ratio}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target} | tee -a ${output}

#done

set +x


