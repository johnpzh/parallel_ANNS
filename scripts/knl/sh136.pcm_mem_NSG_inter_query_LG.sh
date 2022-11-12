#!/usr/local/bin/zsh

export KMP_AFFINITY="granularity=fine,compact,1,0"

#data_dir="/scratch/zpeng/data"
data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
app_tag="NSG_inter_query_pcm_mem"
num_t=64
#bin="python3 ../scripts/test48.find_L_simple_search.py ./pcm_mem_NSG_inter_query_LG"
bin="./pcm_mem_NSG_inter_query_LG"
output="output_${app_tag}.txt"
:> ${output}
set -x

K=100
#P_target=0.999
###################
### SIFT1M
###################
#dataset=sift1m
#L=427
##L_upper=428
#{
#    echo ""
#    echo "###############"
#    echo "## ${dataset}"
#    echo "###############"
#} >> ${output}
#
#eval ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg ${L} ${K} output.ivecs ${data_dir}/sift1m/sift.true-100_NN.v2.binary ${num_t} | tee -a ${output}
#
###################
### DEEP100M
###################
#dataset=deep100m
#L=4399
#{
#    echo ""
#    echo "###############"
#    echo "## ${dataset}"
#    echo "###############"
#} >> ${output}
#
#eval ${bin} ${data_dir}/deep1b/deep100M_base.fvecs ${data_dir}/deep1b/deep100M_query.fvecs ${data_dir}/deep1b/deep100M.nsg ${L} ${K} output.ivecs ${data_dir}/deep1b/deep100M.true-100_NN.v2.binary ${num_t} | tee -a ${output}

##################
## GIST1M
##################
dataset=gist1m
L=2280
{
    echo ""
    echo "###############"
    echo "## ${dataset}"
    echo "###############"
} >> ${output}

eval ${bin} ${data_dir}/gist1m/gist_base.fvecs ${data_dir}/gist1m/gist_query.fvecs ${data_dir}/gist1m/gist.nsg ${L} ${K} output.ivecs ${data_dir}/gist1m/gist.true-100_NN.v2.binary ${num_t} | tee -a ${output}

##################
## DEEP10M
##################
dataset=deep10m
L=994
{
    echo ""
    echo "###############"
    echo "## ${dataset}"
    echo "###############"
} >> ${output}

eval ${bin} ${data_dir}/deep1b/deep10M_base.fvecs ${data_dir}/deep1b/deep10M_query.fvecs ${data_dir}/deep1b/deep10M.nsg ${L} ${K} output.ivecs ${data_dir}/deep1b/deep10M.true-100_NN.v2.binary ${num_t} | tee -a ${output}

##################
## SIFT100M
##################
dataset=sift100m
L=1039
{
    echo ""
    echo "###############"
    echo "## ${dataset}"
    echo "###############"
} >> ${output}

eval ${bin} ${data_dir}/sift1b/sift100M_base.fvecs ${data_dir}/sift1b/sift100M_query.fvecs ${data_dir}/sift1b/sift100M.nsg ${L} ${K} output.ivecs ${data_dir}/sift1b/sift100M.true-100_NN.v2.binary ${num_t} | tee -a ${output}

set +x


