#!/usr/local/bin/zsh

#data_dir="/scratch/zpeng/data"
data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
#app_tag="worker_std_deviation"
app_tag="NSG_pcm_mem"
#num_t=32
#bin="python ../scripts/test78.PSS_v5_work_std_deviation_ranged_L.py ./PSS_v5_worker_std_deviation_LG"
#bin="./pcm_mem_NSG_find_L_LG"
bin="python3 ../scripts/test48.find_L_simple_search.py ./pcm_mem_NSG_find_L_LG"
output="output_${app_tag}.txt"
:> ${output}
set -x

P_target=0.999
##################
## SIFT1M
##################
dataset=sift1m
L_lower=427
L_upper=428
{
    echo ""
    echo "###############"
    echo "## ${dataset}"
    echo "###############"
} >> ${output}
eval ${bin} ${data_dir} ${dataset} ${app_tag} ${L_lower} ${L_upper} ${P_target} | tee -a ${output}

##################
## DEEP100M
##################
dataset=deep100m
L_lower=4399
L_upper=4399
{
    echo ""
    echo "###############"
    echo "## ${dataset}"
    echo "###############"
} >> ${output}
eval ${bin} ${data_dir} ${dataset} ${app_tag} ${L_lower} ${L_upper} ${P_target} | tee -a ${output}

##################
## GIST1M
##################
dataset=gist1m
L_lower=2280
L_upper=2281
{
    echo ""
    echo "###############"
    echo "## ${dataset}"
    echo "###############"
} >> ${output}
eval ${bin} ${data_dir} ${dataset} ${app_tag} ${L_lower} ${L_upper} ${P_target} | tee -a ${output}

##################
## DEEP10M
##################
dataset=deep10m
L_lower=994
L_upper=995
{
    echo ""
    echo "###############"
    echo "## ${dataset}"
    echo "###############"
} >> ${output}
eval ${bin} ${data_dir} ${dataset} ${app_tag} ${L_lower} ${L_upper} ${P_target} | tee -a ${output}

##################
## SIFT100M
##################
dataset=sift100m
L_lower=1039
L_upper=1040
{
    echo ""
    echo "###############"
    echo "## ${dataset}"
    echo "###############"
} >> ${output}
eval ${bin} ${data_dir} ${dataset} ${app_tag} ${L_lower} ${L_upper} ${P_target} | tee -a ${output}


set +x


