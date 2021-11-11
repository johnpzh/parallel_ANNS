#!/usr/local/bin/zsh

function get_L_low () {
    if (($1 < 100)); then
        echo $1
    elif (( ($1 / $2) < 100)); then
        echo 100
    else
        echo $(($1 / $2))
    fi
}

function get_X_low () {
    if (( ($1 / $2) < 1 )); then
        echo 1
    else
        echo $(( $1 / $2))
    fi
}

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
data=deep100m

####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test67.PSS_v5_count_reorder_ranged_L.py"
app="PSS_v5_LG_degree_reorder"
app_tag="PSS_v5_reorder"
################################
#### Latency
################################

reorder_mode="expand"
#for num_t in 1 2 4 8 16 32 64; do
opt_id_thrd=100000
P_target=0.999
L_Seq=3339
#for opt_id_thrd in 100000; do
#for opt_id_thrd in 1000000; do
#for opt_id_thrd in 10 100 1000 10000 100000 1000000 10000000; do
#    P_target=0.999

num_t=2
L_low=1669
L_up=1669
L_step=8
X_low=1685
X_up=1701
X_step=16
tag="${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd} ${reorder_mode}
table_file="output.${data}.${tag}.table.txt"
selected_file="output.${data}.${tag}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

num_t=4
L_low=834
L_up=834
L_step=8
X_low=842
X_up=850
X_step=8
tag="${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd} ${reorder_mode}
table_file="output.${data}.${tag}.table.txt"
selected_file="output.${data}.${tag}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

num_t=8
L_low=417
L_up=417
L_step=8
X_low=433
X_up=441
X_step=8
tag="${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd} ${reorder_mode}
table_file="output.${data}.${tag}.table.txt"
selected_file="output.${data}.${tag}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

#done

cat output.${data}.${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT*.selected.txt > output.${data}.${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_collected.selected.txt
#done

reorder_mode="in_degree"
#for num_t in 1 2 4 8 16 32 64; do
opt_id_thrd=1000
P_target=0.999
L_Seq=3339
#for opt_id_thrd in 10000; do
#for opt_id_thrd in 1000000; do
#for opt_id_thrd in 10 100 1000 10000 100000 1000000 10000000; do
#    P_target=0.999

num_t=2
L_low=1669
L_up=1669
L_step=8
X_low=1685
X_up=1701
X_step=16
tag="${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd} ${reorder_mode}
table_file="output.${data}.${tag}.table.txt"
selected_file="output.${data}.${tag}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

num_t=4
L_low=834
L_up=834
L_step=8
X_low=842
X_up=850
X_step=8
tag="${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd} ${reorder_mode}
table_file="output.${data}.${tag}.table.txt"
selected_file="output.${data}.${tag}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

num_t=8
L_low=417
L_up=417
L_step=8
X_low=433
X_up=441
X_step=8
tag="${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd} ${reorder_mode}
table_file="output.${data}.${tag}.table.txt"
selected_file="output.${data}.${tag}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

#done

cat output.${data}.${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT*.selected.txt > output.${data}.${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_collected.selected.txt
#done

set +x


