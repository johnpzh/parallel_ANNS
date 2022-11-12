#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
data=deep100m

####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py"
app="PSS_v5_LG_distance_threshold_profiling"

app_tag="PSS_v5_bdw"
################################
#### DEEP100M
################################

#for num_t in 32; do
num_t=64
P_target=0.999
L_low=104
L_up=104
L_step=1
X_low=112
X_up=113
X_step=1
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0


set +x


