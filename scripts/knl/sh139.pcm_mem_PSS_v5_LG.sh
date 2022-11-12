#!/usr/local/bin/zsh
set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
bin="python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py"
app="pcm_mem_PSS_v5_LG"

app_tag="PSS_v5_pcm_mem"
output="output_${app_tag}.txt"
:> ${output}
#num_t=64
num_t=32
P_target=0.999
################################
#### SIFT1M
################################
data=sift1m
{
    echo ""
    echo "###############"
    echo "## ${data}"
    echo "###############"
} >> ${output}

L_low=100
L_up=100
L_step=1
X_low=17
X_up=18
X_step=1
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}


################################
#### DEEP100M
################################
data=deep100m
{
    echo ""
    echo "###############"
    echo "## ${data}"
    echo "###############"
} >> ${output}

L_low=104
L_up=105
L_step=1
X_low=112
X_up=113
X_step=1
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}

################################
#### GIST1M
################################
data=gist1m
{
    echo ""
    echo "###############"
    echo "## ${data}"
    echo "###############"
} >> ${output}

L_low=108
L_up=109
L_step=1
X_low=77
X_up=78
X_step=1
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}

################################
#### DEEP10M
################################
data=deep10m
{
    echo ""
    echo "###############"
    echo "## ${data}"
    echo "###############"
} >> ${output}

L_low=116
L_up=117
L_step=1
X_low=35
X_up=36
X_step=1
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}

################################
#### SIFT100M
################################
data=sift100m
{
    echo ""
    echo "###############"
    echo "## ${data}"
    echo "###############"
} >> ${output}

L_low=100
L_up=101
L_step=1
X_low=39
X_up=40
X_step=1
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}

set +x


