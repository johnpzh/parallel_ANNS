#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
bin="pcm-memory.x -- python3 ../scripts/test74.PSS_v5_find_L_every_step_merge.py"
#bin="python3 ../scripts/test74.PSS_v5_find_L_every_step_merge.py"
app="pcm_mem_PathWise_every_step_merge_LG"
X=1
app_tag="PathWise_every_step_merge_mem_bdw"
num_t=32
output="output_${app_tag}.txt"
:> ${output}
###########
# DEEP100M
###########
data=deep100m
##
#P_target=0.999
#tag="${app_tag}_T${num_t}_P${P_target}"
#L_low=80
#L_up=3300
#P_ts="0.900 0.990 0.995 0.997 0.999"
##P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}

#############
## 0.994
#############
P_target=0.994
tag="${app_tag}_T${num_t}_P${P_target}"
L_low=94
L_up=1000
P_ts="0.9 0.91 0.92 0.93 0.94 \
0.95 0.96 0.97 0.98 0.99 \
0.991 0.992 0.993 0.994"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts} | tee -a ${output}

#############
## 0.996
#############
P_target=0.996
tag="${app_tag}_T${num_t}_P${P_target}"
L_low=1000
L_up=1548
P_ts="0.995 0.996"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts} | tee -a ${output}
#############
## 0.998
#############
P_target=0.998
tag="${app_tag}_T${num_t}_P${P_target}"
L_low=1333
L_up=3125
P_ts="0.997 0.998"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts} | tee -a ${output}
#############
## 0.999
#############
P_target=0.999
tag="${app_tag}_T${num_t}_P${P_target}"
L_low=3124
L_up=3126
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts} | tee -a ${output}

############
## SIFT100M
############
#data=sift100m
###
#P_target=0.999
#tag="${app_tag}_T${num_t}_P${P_target}"
#L_low=80
#L_up=1000
#P_ts="0.900 0.990 0.995 0.997 0.999"
##P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts} | tee -a ${output}

set +x


