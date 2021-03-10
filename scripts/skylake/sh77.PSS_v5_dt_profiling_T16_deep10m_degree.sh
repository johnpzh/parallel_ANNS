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
data=deep10m
data_name=deep10M


####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test66.PSS_v5_dt_profiling_ranged_L_degree.py"
app="PSS_v5_distance_threshold_profiling"
#app="profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling"
app_tag="PSS_v5_dt"

for num_t in 16; do

#    data_dir="${data_dir}/deep1b"
    P_target=0.999
#    L_Seq=890
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 16))

    #### R16
    R=16
    L_low=1591
    L_up=1593
    L_step=1
    #    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
    #    X_up=$((X_low + 16))
    X_low=1591
    X_up=1593
    X_step=1
    tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${R}
    #######
    table_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999

#    #### R32
#    R=32
#    L_low=135
#    L_up=138
#    L_step=1
#    #    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    #    X_up=$((X_low + 16))
#    X_low=135
#    X_up=138
#    X_step=1
#    tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${R}
#    #######
#    table_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999

#    #### R64
#    R=64
#    L_low=100
#    L_up=100
#    L_step=8
#    #    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    #    X_up=$((X_low + 16))
#    X_low=53
#    X_up=55
#    X_step=1
#    tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${R}
#    #######
#    table_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.910 0.920 0.930 0.940 \
##    0.950 0.960 0.970 0.980 0.990 \
##    0.991 0.992 0.993 0.994 0.995 \
##    0.996 0.997 0.998 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.990 0.995 0.997 0.999
#
#    #### R128
#    R=128
#    L_low=100
#    L_up=100
#    L_step=8
#    #    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    #    X_up=$((X_low + 16))
#    X_low=49
#    X_up=51
#    X_step=1
#    tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${R}    #######
#    table_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.910 0.920 0.930 0.940 \
##    0.950 0.960 0.970 0.980 0.990 \
##    0.991 0.992 0.993 0.994 0.995 \
##    0.996 0.997 0.998 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.990 0.995 0.997 0.999
#
#    #### R256
#    R=256
#    L_low=100
#    L_up=100
#    L_step=8
#    #    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    #    X_up=$((X_low + 16))
#    X_low=49
#    X_up=51
#    X_step=1
#    tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${R}    #######
#    table_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.910 0.920 0.930 0.940 \
##    0.950 0.960 0.970 0.980 0.990 \
##    0.991 0.992 0.993 0.994 0.995 \
##    0.996 0.997 0.998 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.990 0.995 0.997 0.999
#
#    #### R512
#    R=512
#    L_low=100
#    L_up=100
#    L_step=8
#    #    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    #    X_up=$((X_low + 16))
#    X_low=38
#    X_up=40
#    X_step=1
#    tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${R}    #######
#    table_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.910 0.920 0.930 0.940 \
##    0.950 0.960 0.970 0.980 0.990 \
##    0.991 0.992 0.993 0.994 0.995 \
##    0.996 0.997 0.998 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.990 0.995 0.997 0.999
#
#    #### R1024
#    R=1024
#    L_low=100
#    L_up=100
#    L_step=8
#    #    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    #    X_up=$((X_low + 16))
#    X_low=33
#    X_up=35
#    X_step=1
#    tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${R}    #######
#    table_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_R${R}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.910 0.920 0.930 0.940 \
##    0.950 0.960 0.970 0.980 0.990 \
##    0.991 0.992 0.993 0.994 0.995 \
##    0.996 0.997 0.998 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.990 0.995 0.997 0.999

done

set +x


