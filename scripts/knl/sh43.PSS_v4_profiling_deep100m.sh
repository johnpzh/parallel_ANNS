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

##################
#### PSS 1T
##################
bin="python3 ../scripts/test46.find_L_PSS_v4_profile_T1.py"
app="profile_find_L_para_single_query_search_PSS_v4_large_graph_profile"

num_t=1
##
P_target=0.900
tag="PSS_T${num_t}_P${P_target}"
L_low=110
L_up=500
P_ts="0.9"
#P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.990
tag="PSS_T${num_t}_P${P_target}"
L_low=700
L_up=1200
P_ts="0.99 0.995"
#P_ts="0.99 0.991 0.992 0.993 0.994 0.995"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.996
tag="PSS_T${num_t}_P${P_target}"
L_low=1300
L_up=1650
P_ts="0.997"
#P_ts="0.996 0.997"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

#P_target=0.998
#tag="PSS_T${num_t}_P${P_target}"
#L_low=2050
#L_up=2150
#P_ts="0.998"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.999
tag="PSS_T${num_t}_P${P_target}"
L_low=3300
L_up=3500
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

output_files="output.${data}.PSS_T${num_t}_P0.9"
table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

#####################################
##### PSS Multiple Threads
#####################################
#bin="python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py"
#app="profile_para_single_query_search_PSS_v4_large_graph_ranged_profiling"
#
#####################
###### DEEP100M 2T
#####################
#for num_t in 2 4 8 16 32; do
####
#    P_target=0.900
#    L_Seq=120
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.910
##    L_Seq=131
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.920
##    L_Seq=147
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.930
##    L_Seq=163
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.940
##    L_Seq=188
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.950
##    L_Seq=217
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.960
##    L_Seq=260
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 2))
##    L_step=1
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 2))
##    X_step=1
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.970
##    L_Seq=325
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 4))
##    L_step=2
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 4))
##    X_step=2
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.980
##    L_Seq=442
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 4))
##    L_step=2
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 4))
##    X_step=2
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.990
#    L_Seq=710
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 16))
#    L_step=8
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 16))
#    X_step=8
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.991
##    L_Seq=764
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.992
##    L_Seq=833
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.993
##    L_Seq=910
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.994
##    L_Seq=1011
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.995
#    L_Seq=1142
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 16))
#    L_step=8
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 16))
#    X_step=8
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.996
##    L_Seq=1335
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.997
#    L_Seq=1605
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 16))
#    L_step=8
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 16))
#    X_step=8
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
##
##    P_target=0.998
##    L_Seq=2100
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((L_low + 16))
##    L_step=8
##    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
##    X_up=$((X_low + 16))
##    X_step=8
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.999
#    L_Seq=3339
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 16))
#    L_step=8
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 16))
#    X_step=8
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    ######
#    output_files="output.${data}.PSS_T${num_t}_P0.9"
#    table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
#    selected_file="output.${data}.PSS_T${num_t}_collected.selected.txt"
#    cat ${output_files}*.table.txt > ${table_file}
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.910 0.920 0.930 0.940 \
##    0.950 0.960 0.970 0.980 0.990 \
##    0.991 0.992 0.993 0.994 0.995 \
##    0.996 0.997 0.998 0.999
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
#    0.900 0.990 0.995 0.997 0.999
#
#done


set +x


