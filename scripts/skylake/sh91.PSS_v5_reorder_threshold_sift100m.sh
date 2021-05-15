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

#data_dir="/home/zpeng/data"
data_dir="/scratch/zpeng"
data=sift100m

####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test67.PSS_v5_count_reorder_ranged_L.py"
app="PSS_v5_LG_degree_reorder"
app_tag="PSS_v5_reorder"
################################
#### Latency
################################

#for reorder_mode in "expand" "in_degree"; do
#
#for num_t in 1 2 4 8 16; do
##for num_t in 2 4 8 16 32; do
#P_target=0.999
#for opt_id_thrd in 100000; do
##for opt_id_thrd in 100000; do
##for opt_id_thrd in 10 100 1000 10000 100000 1000000 10000000; do
##    P_target=0.999
#    L_Seq=999
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 16))
#    L_step=8
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 16))
#    X_step=8
#    tag="${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd} ${reorder_mode}
#    table_file="output.${data}.${tag}.table.txt"
#    selected_file="output.${data}.${tag}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#done
#
#cat output.${data}.${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT*.selected.txt > output.${data}.${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_collected.selected.txt
#done
#
#done


for reorder_mode in "in_degree"; do
#for reorder_mode in "expand" "in_degree"; do

for num_t in 8; do
#for num_t in 8 16; do
P_target=0.999
for opt_id_thrd in 290000 280000 270000 260000 250000 240000 230000 220000 210000; do
#for opt_id_thrd in 390000 380000 370000 360000 350000 340000 330000 320000 310000 300000 290000; do
#for opt_id_thrd in 1000000 900000 800000 700000 600000 500000 400000 300000 200000; do
#for opt_id_thrd in 100000; do
#for opt_id_thrd in 10 100 1000 10000 100000 1000000 10000000; do
#    P_target=0.999
    L_Seq=999
    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
    L_up=$((L_low + 8))
    L_step=8
    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
    X_up=$((X_low + 8))
    X_step=1
    tag="${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd} ${reorder_mode}
    table_file="output.${data}.${tag}.table.txt"
    selected_file="output.${data}.${tag}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done

cat output.${data}.${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_OPT*.selected.txt > output.${data}.${app_tag}_${reorder_mode}_T${num_t}_P${P_target}_collected.selected.txt
done

done


set +x


