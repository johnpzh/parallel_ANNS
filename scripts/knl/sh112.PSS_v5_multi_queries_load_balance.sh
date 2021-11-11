#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
#data=sift1m

####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py"
app="PSS_v5_multi_queries_load_balance"
app_tag="PSS_v5_multi_queries_load_balance"

################################
#### SIFT1M
################################
data=sift1m
P_target=0.999
L_low=100
L_up=100
L_step=1
X_low=54
X_up=57
X_step=1

#for num_t in 8; do
for num_t in 8 16 32 64; do
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done

################################
#### GIST1M
################################
data=gist1m
P_target=0.999
L_low=278
L_up=278
L_step=1
X_low=292
X_up=295
X_step=1

#for num_t in 8; do
for num_t in 8 16 32 64; do
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done

################################
#### DEEP10M
################################
data=deep10m
P_target=0.999
L_low=119
L_up=119
L_step=1
X_low=117
X_up=120
X_step=1

#for num_t in 8; do
for num_t in 8 16 32 64; do
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done



set +x


