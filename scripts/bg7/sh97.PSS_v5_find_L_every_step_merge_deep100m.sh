#!/usr/local/bin/zsh

set -x

data_dir="/scratch/zpeng/data"
#data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
bin="python3 ../scripts/test74.PSS_v5_find_L_every_step_merge.py"
app="PSS_v5_find_L_every_step_merge_LG"
num_t=32
#X=8
#for X in 8 32 64 128; do
#for X in 8 16 32 64 128; do

app_tag="PSS_v5_every_step_merge"


###########
# DEEP100M
###########
data=deep100m
P_ts="0.999"
P_target=0.999
##
X=1
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=3125
L_up=3127
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
##
X=2
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=3318
L_up=3320
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
##
X=4
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=3311
L_up=3313
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
##
X=8
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=3304
L_up=3304
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
##
X=16
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=3260
L_up=3262
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
##
X=32
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=3257
L_up=3259
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
##
X=64
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=2960
L_up=2962
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
##
X=128
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=108
L_up=110
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
##
X=256
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_low=107
L_up=109
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}

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
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${X} ${P_ts}
#done
set +x


