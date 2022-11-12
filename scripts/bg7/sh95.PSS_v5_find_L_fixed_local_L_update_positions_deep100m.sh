#!/usr/local/bin/zsh

set -x

data_dir="/scratch/zpeng/data"
#data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
bin="python3 ../scripts/test79.PSS_v5_find_L_every_step_merge_fixed_local_L.py"
app="PSS_v5_find_L_fixed_local_L_update_positions_LG"
num_t=32
#X=8
for X in 1 2 4 8 16 32 64 128 256; do

app_tag="intervals_vs_update_pos"


###########
# DEEP100M
###########
data=deep100m
##
P_target=0.999
tag="${app_tag}_X${X}_T${num_t}_P${P_target}"
L_master_low=80
L_master_up=4000
L_local=100
P_ts="0.999"
#P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master_low} ${L_master_up} ${L_local} ${X} ${P_ts}

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
done
set +x


