#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
bin="python3 ../scripts/test75.PSS_v5_find_L_every_step_merge_L_master.py"
app="PSS_v5_find_L_no_staged_LG"
X=999999999
app_tag="PSS_v5_no_staged"
num_t=16
#L_master=102
#L_master=100
###########
# DEEP100M
###########
data=deep100m
##
#P_target=0.997
#tag="${app_tag}_T${num_t}_P${P_target}"

#L_master=100
#L_local_low=5
#L_local_up=200
#
#P_ts="0.9 0.91 0.92 0.93 0.94 \
#      0.95 0.96 0.97 0.98 0.99 \
#      0.991 0.992 0.993 0.994 0.995 \
#      0.996 0.997"
##P_ts="0.9 0.91 0.92 0.93 0.94 \
##      0.95 0.96 0.97 0.98 0.99 \
##      0.991 0.992 0.993 0.994 0.995 \
##      0.996 0.997 0.998 0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_low} ${L_local_up} ${X} ${P_ts}

#############
## 0.998
#############
P_target=0.998
tag="${app_tag}_T${num_t}_P${P_target}"
L_master=131
L_local_low=100
L_local_up=200

P_ts="0.998"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_low} ${L_local_up} ${X} ${P_ts}

#############
## 0.999
#############
P_target=0.999
tag="${app_tag}_T${num_t}_P${P_target}"
L_master=208
L_local_low=150
L_local_up=300

P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_low} ${L_local_up} ${X} ${P_ts}


############
## SIFT100M
############
#data=sift100m
###
#P_target=0.999
#tag="${app_tag}_T${num_t}_P${P_target}"
#L_local_low=5
#L_local_up=200
#P_ts="0.900 0.990 0.995 0.997 0.999"
##P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_low} ${L_local_up} ${X} ${P_ts}

set +x


