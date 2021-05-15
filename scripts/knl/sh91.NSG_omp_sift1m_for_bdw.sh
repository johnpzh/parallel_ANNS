#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
data=sift1m

###############
#### SGS
###############
bin="python3 ../scripts/test64.find_L_NSG_omp.py"
app="profile_find_L_para_single_query_NSG_omp"
app_label="NSG_OMP"
num_t=64

#P_target=0.900
#tag="${app_label}_T${num_t}_P${P_target}"
#L_low=90
#L_up=100
#P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
#
#P_target=0.970
#tag="${app_label}_T${num_t}_P${P_target}"
#L_low=100
#L_up=200
#P_ts="0.97 0.98 0.99 0.991 0.992"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
#
#P_target=0.993
#tag="${app_label}_T${num_t}_P${P_target}"
#L_low=200
#L_up=300
#P_ts="0.993 0.994 0.995 0.996 0.997"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
#
#P_target=0.998
#tag="${app_label}_T${num_t}_P${P_target}"
#L_low=300
#L_up=350
#P_ts="0.998"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.999
tag="${app_label}_T${num_t}_P${P_target}"
L_low=428
L_up=428
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

######
output_files="output.${data}.${app_label}_T${num_t}_P0.9"
table_file="output.${data}.${app_label}_T${num_t}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


