#!/usr/local/bin/zsh

set -x

#data_dir="/home/zpeng/data"
data_dir="/scratch/zpeng"
data=gist1m

###############
#### SGS
###############
bin="python3 ../scripts/test64.find_L_NSG_omp.py"
app="profile_find_L_para_single_query_NSG_omp"
app_label="NSG_OMP"
num_t=16

P_target=0.900
tag="${app_label}_T${num_t}_P${P_target}"
L_low=110
L_up=200
P_ts="0.9 0.91 0.92 0.93 0.94"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.950
tag="${app_label}_T${num_t}_P${P_target}"
L_low=210
L_up=260
P_ts="0.95 0.96"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.970
tag="${app_label}_T${num_t}_P${P_target}"
L_low=300
L_up=400
P_ts="0.97 0.98"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.990
tag="${app_label}_T${num_t}_P${P_target}"
L_low=600
L_up=700
P_ts="0.99 0.991 0.992"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.993
tag="${app_label}_T${num_t}_P${P_target}"
L_low=710
L_up=740
P_ts="0.993"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.994
tag="${app_label}_T${num_t}_P${P_target}"
L_low=790
L_up=810
P_ts="0.994"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.995
tag="${app_label}_T${num_t}_P${P_target}"
L_low=890
L_up=910
P_ts="0.995"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.996
tag="${app_label}_T${num_t}_P${P_target}"
L_low=1000
L_up=1040
P_ts="0.996"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.997
tag="${app_label}_T${num_t}_P${P_target}"
L_low=1150
L_up=1200
P_ts="0.997"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.998
tag="${app_label}_T${num_t}_P${P_target}"
L_low=1500
L_up=1600
P_ts="0.998"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

P_target=0.999
tag="${app_label}_T${num_t}_P${P_target}"
L_low=2200
L_up=2300
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}

######
output_files="output.${data}.${app_label}_T${num_t}_P0.9"
table_file="output.${data}.${app_label}_T${num_t}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


