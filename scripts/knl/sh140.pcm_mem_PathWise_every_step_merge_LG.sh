#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
bin="python3 ../scripts/test74.PSS_v5_find_L_every_step_merge.py"
app="pcm_mem_PathWise_every_step_merge_LG"
X=1
app_tag="PathWise_every_step_merge_pcm_mem"
#num_t=64
num_t=32
P_target=0.999
output="output_${app_tag}.txt"
:> ${output}
###########
# DEEP100M
###########
data=deep100m
{
    echo ""
    echo "###############"
    echo "## ${data}"
    echo "###############"
} >> ${output}

#P_target=0.999

#for num_t in 32 64 16; do

tag="${app_tag}_T${num_t}_P${P_target}"
L_low=3125
L_up=3125
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts} | tee -a ${output}

#done

set +x


