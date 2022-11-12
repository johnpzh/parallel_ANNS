#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"


bin="python3 ../scripts/test64.find_L_NSG_omp.py"
app="pcm_mem_NSG_omp_find_LG"
app_label="NSG_OMP_pcm_mem"
output="output_${app_label}.txt"
:> ${output}

for num_t in 32 16 8; do

P_target=0.999
tag="${app_label}_T${num_t}_P${P_target}"
P_ts="0.999"
###################
### SIFT1M
###################
#data=sift1m
#L_low=427
#L_up=428
#{
#    echo ""
#    echo "###############"
#    echo "## ${data}"
#    echo "###############"
#} >> ${output}
#
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts} | tee -a ${output}

##################
## DEEP100M
##################
data=deep100m
L_low=4399
L_up=4399
{
    echo ""
    echo "###############"
    echo "## ${data}"
    echo "###############"
} >> ${output}

eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts} | tee -a ${output}

done
###################
### GIST1M
###################
#data=gist1m
#L_low=2280
#L_up=2281
#{
#    echo ""
#    echo "###############"
#    echo "## ${data}"
#    echo "###############"
#} >> ${output}
#
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts} | tee -a ${output}
#
###################
### DEEP10M
###################
#data=deep10m
#L_low=994
#L_up=995
#{
#    echo ""
#    echo "###############"
#    echo "## ${data}"
#    echo "###############"
#} >> ${output}
#
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts} | tee -a ${output}
#
###################
### SIFT100M
###################
#data=sift100m
#L_low=1039
#L_up=1040
#{
#    echo ""
#    echo "###############"
#    echo "## ${data}"
#    echo "###############"
#} >> ${output}
#
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts} | tee -a ${output}


set +x


