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
bin="pcm-memory.x -- python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py"
app="pcm_mem_PSS_v5_LG"

#app_tag="PSS_v5_dt_RT"

app_tag="PSS_v5_mem_bdw"
output="output_${app_tag}.txt"
:> ${output}
################################
#### Latency
################################

for num_t in 32; do
#for num_t in 2 4 8 16 32 64; do
##
    P_target=0.900
    L_low=100
    L_up=100
    L_step=1
    X_low=4
    X_up=4
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.910
    L_low=100
    L_up=100
    L_step=1
    X_low=4
    X_up=4
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.920
    L_low=100
    L_up=100
    L_step=1
    X_low=4
    X_up=4
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.930
    L_low=100
    L_up=100
    L_step=1
    X_low=4
    X_up=4
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.940
    L_low=100
    L_up=100
    L_step=1
    X_low=6
    X_up=6
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.950
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=12
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.960
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=14
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.970
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=17
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.980
    L_low=108
    L_up=${L_low}
    L_step=1
    X_low=23
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.990
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=31
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.991
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=31
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.992
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=34
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.993
    L_low=116
    L_up=${L_low}
    L_step=1
    X_low=35
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.994
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=39
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.995
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=43
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.996
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=51
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.997
    L_low=116
    L_up=${L_low}
    L_step=1
    X_low=58
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.998
    L_low=100
    L_up=${L_low}
    L_step=1
    X_low=81
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    P_target=0.999
    L_low=104
    L_up=${L_low}
    L_step=1
    X_low=112
    X_up=${X_low}
    X_step=1
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 | tee -a ${output}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

    #######
    collected_selected="output.${data}.${app_tag}_T${num_t}_collected.selected_0.txt"
    cat output.${data}.${app_tag}_T${num_t}_P0.9*.selected.txt > ${collected_selected}
    #######
    output_files="output.${data}.${app_tag}_T${num_t}_P0.9"
    table_collected="output.${data}.${app_tag}_T${num_t}_collected.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_collected.selected_1.txt"
    cat ${output_files}*.table.txt > ${table_collected}
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
    0.900 0.910 0.920 0.930 0.940 \
    0.950 0.960 0.970 0.980 0.990 \
    0.991 0.992 0.993 0.994 0.995 \
    0.996 0.997 0.998 0.999
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
#    0.900 0.990 0.995 0.997 0.999

done

#app_tag="PSS_v5_dt_Scal"
#################################
##### Scalability
#################################
#
##for num_t in 16; do
#for num_t in 16 32 64; do
##for num_t in 2 4 8 16 32 64; do
####
##    P_target=0.900
##    L_Seq=120
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
##    P_target=0.910
##    L_Seq=131
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
##    L_Seq=147
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
##    L_Seq=163
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
##    L_Seq=188
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
##    L_Seq=217
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
##    L_Seq=260
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
##    L_Seq=325
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
##
##    P_target=0.980
##    L_Seq=442
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
##    P_target=0.990
##    L_Seq=710
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.991
##    L_Seq=764
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.992
##    L_Seq=833
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.993
##    L_Seq=910
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
##
##    P_target=0.994
##    L_Seq=1011
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
##    P_target=0.995
##    L_Seq=1142
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
##    P_target=0.996
##    L_Seq=1335
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
##    P_target=0.997
##    L_Seq=1605
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
##    P_target=0.998
##    L_Seq=2100
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="${app_tag}_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
##    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
##    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#    P_target=0.999
#    L_Seq=3339
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t})))
#    L_up=$((L_low + 16))
#    L_step=8
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t})))
#    X_up=$((X_low + 16))
#    X_step=8
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


