#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
data=sift1m

###############
#### SIFT1M
###############
bin="python3 ../scripts/test64.find_L_NSG_omp.py"
#app="NSG_omp_profile_multi_queues"
app="profile_find_L_para_single_query_NSG_omp_profile"
app_label="insert_NSG_OMP"

###############
## 0.999
###############
P_target=0.999
#for num_t in 32 64; do
for num_t in 1 2 4 8 16 32 64; do
    tag="${app_label}_P${P_target}_T${num_t}"
    L_low=428
    L_up=428
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
    L_low=173
    L_up=173
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
    L_low=91
    L_up=91
    P_ts="0.9"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${num_t} ${P_ts}
done

output_files="output.${data}.${app_label}_P${P_target}_T"
table_file="output.${data}.${app_label}_P${P_target}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


