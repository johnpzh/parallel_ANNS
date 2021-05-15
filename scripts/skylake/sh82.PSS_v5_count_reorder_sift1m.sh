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
data=sift1m


####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test67.PSS_v5_count_reorder_ranged_L.py"
app="PSS_v5_LG_count_reorder"

app_tag="PSS_v5_count_reorder"
################################
#### Latency
################################

#for num_t in 2 4 8 16 32; do
for num_t in 16; do
##
#    P_target=0.900
#    L_Seq=90
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.910
#    L_Seq=91
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.920
#    L_Seq=92
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.930
#    L_Seq=93
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.940
#    L_Seq=95
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.950
#    L_Seq=96
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.960
#    L_Seq=98
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.970
#    L_Seq=105
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.980
#    L_Seq=128
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.990
#    L_Seq=173
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.991
#    L_Seq=181
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.992
#    L_Seq=190
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.993
#    L_Seq=201
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.994
#    L_Seq=214
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.995
#    L_Seq=231
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.996
#    L_Seq=253
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.997
#    L_Seq=282
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.998
#    L_Seq=330
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
P_target=0.999
for opt_id_thrd in 1 10 100; do
#for opt_id_thrd in 1 10 100 1000 10000 100000 1000000 10000000; do
    P_target=0.999
    L_Seq=428
    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
    L_up=$((L_low + 8))
    L_step=4
    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
    X_up=$((X_low + 8))
    X_step=4
    tag="${app_tag}_T${num_t}_P${P_target}_OPT${opt_id_thrd}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} ${opt_id_thrd}
    table_file="output.${data}.${tag}.table.txt"
    selected_file="output.${data}.${tag}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

#    #######
#    collected_selected="output.${data}.${app_tag}_T${num_t}_P${P_target}_collected.selected_0.txt"
#    cat output.${data}.${app_tag}_T${num_t}_P${P_target}_OPT*.selected.txt > ${collected_selected}
#    #######
#    output_files="output.${data}.${app_tag}_T${num_t}_P${P_target}_OPT"
#    table_collected="output.${data}.${app_tag}_T${num_t}_P${P_target}_collected.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}_collected.selected_1.txt"
#    cat ${output_files}*.table.txt > ${table_collected}
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
##    0.900 0.910 0.920 0.930 0.940 \
##    0.950 0.960 0.970 0.980 0.990 \
##    0.991 0.992 0.993 0.994 0.995 \
##    0.996 0.997 0.998 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
##    0.900 0.990 0.995 0.997 0.999
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
#    0.999
done

cat output.${data}.${app_tag}_T${num_t}_P${P_target}_OPT*.selected.txt > output.${data}.${app_tag}_T${num_t}_P${P_target}_collected.selected.txt
done

#app_tag="PSS_v5_dt_Scal"
#################################
##### Scalability
#################################
#
#for num_t in 2 4 8 16 20; do
##for num_t in 16; do
###
#    P_target=0.900
#    L_Seq=90
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
##    P_target=0.910
##    L_Seq=91
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.920
##    L_Seq=92
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.930
##    L_Seq=93
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.940
##    L_Seq=95
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.950
##    L_Seq=96
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.960
##    L_Seq=98
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.970
##    L_Seq=105
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.980
##    L_Seq=128
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.990
#    L_Seq=173
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.991
##    L_Seq=181
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.992
##    L_Seq=190
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.993
##    L_Seq=201
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.994
##    L_Seq=214
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 4))
##    L_step=2
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 4))
##    X_step=2
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.995
#    L_Seq=231
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.996
##    L_Seq=253
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 4))
##    L_step=2
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 4))
##    X_step=2
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.997
#    L_Seq=282
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.998
##    L_Seq=330
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 4))
##    L_step=2
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 4))
##    X_step=2
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.999
#    L_Seq=428
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 8))
#    L_step=4
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 8))
#    X_step=4
#    tag="${app_tag}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    #######
#    collected_selected="output.${data}.${app_tag}_T${num_t}_collected.selected_0.txt"
#    cat output.${data}.${app_tag}_T${num_t}_P0.9*.selected.txt > ${collected_selected}
#    #######
#    output_files="output.${data}.${app_tag}_T${num_t}_P0.9"
#    table_collected="output.${data}.${app_tag}_T${num_t}_collected.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_collected.selected_1.txt"
#    cat ${output_files}*.table.txt > ${table_collected}
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
##    0.900 0.910 0.920 0.930 0.940 \
##    0.950 0.960 0.970 0.980 0.990 \
##    0.991 0.992 0.993 0.994 0.995 \
##    0.996 0.997 0.998 0.999
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
#    0.900 0.990 0.995 0.997 0.999
#
#done



set +x


