#!/usr/local/bin/zsh

set -x

data_dir="/scratch/zpeng"
#data_dir="/home/zpeng/data"
data=deep100m

###############
#### DEEP100M
###############
bin="python3 ../scripts/test64.find_L_NSG_omp.py"
#app="NSG_omp_profile_multi_queues_LG"
app="profile_find_L_para_single_query_NSG_omp_large_graph_profile"
app_label="insert_NSG_OMP"
#num_t=32
#
#P_target=0.900
#tag="${app_label}_T${num_t}_P${P_target}"
#L_low=115
#L_up=500
#P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
#
#P_target=0.990
#tag="${app_label}_T${num_t}_P${P_target}"
#L_low=700
#L_up=1850
#P_ts="0.99 0.991 0.992 0.993 0.994 0.995 0.996 0.997"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
#
#P_target=0.998
#tag="${app_label}_T${num_t}_P${P_target}"
#L_low=2350
#L_up=2550
#P_ts="0.998"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
#
#P_target=0.999
#tag="${app_label}_T${num_t}_P${P_target}"
#L_low=4350
#L_up=4550
#P_ts="0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
#
#######
#output_files="output.${data}.${app_label}_T${num_t}_P0.9"
#table_file="output.${data}.${app_label}_T${num_t}_collected.table.txt"
#cat ${output_files}*.table.txt > ${table_file}

###############
## 0.999
###############
P_target=0.999
for num_t in 1 2 4 8 16 32 64; do
    tag="${app_label}_P${P_target}_T${num_t}"
    L_low=4414
    L_up=4414
    P_ts="0.999"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
done

output_files="output.${data}.${app_label}_P${P_target}_T"
table_file="output.${data}.${app_label}_P${P_target}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

###############
## 0.99
###############
P_target=0.99
#for num_t in 32 64; do
for num_t in 1 2 4 8 16 32 64; do
    tag="${app_label}_P${P_target}_T${num_t}"
    L_low=742
    L_up=742
    P_ts="0.99"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
done

output_files="output.${data}.${app_label}_P${P_target}_T"
table_file="output.${data}.${app_label}_P${P_target}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

###############
## 0.9
###############
P_target=0.9
#for num_t in 32 64; do
for num_t in 1 2 4 8 16 32 64; do
    tag="${app_label}_P${P_target}_T${num_t}"
    L_low=121
    L_up=121
    P_ts="0.9"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
done

output_files="output.${data}.${app_label}_P${P_target}_T"
table_file="output.${data}.${app_label}_P${P_target}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


