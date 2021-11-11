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
bin="python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py"
app="PSS_v5_LG_distance_threshold_profiling"
#app="profile_para_single_query_search_PSS_v5_large_graph_dist_thresh_ranged_profiling"
#app_tag="PSS_v5_dt"

app_tag="PSS_v5_Sense_L&X"
################################
#### Latency
################################

#for num_t in 8; do
num_t=8
P_target=0.999
#    L_Seq=3339

app_tag="PSS_v5_Sense_X"
L_low=417
L_up=417
L_step=8
X_low=40
X_up=440
X_step=10
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0

app_tag="PSS_v5_Sense_L"
L_low=20
L_up=420
L_step=10
X_low=433
X_up=433
X_step=10
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
#    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

#    #######
#    collected_selected="output.${data}.${app_tag}_T${num_t}_collected.selected_0.txt"
#    cat output.${data}.${app_tag}_T${num_t}_P0.9*.selected.txt > ${collected_selected}
#    #######
#    output_files="output.${data}.${app_tag}_T${num_t}_P0.9"
#    table_collected="output.${data}.${app_tag}_T${num_t}_collected.table.txt"
#    selected_file="output.${data}.${app_tag}_T${num_t}_collected.selected_1.txt"
#    cat ${output_files}*.table.txt > ${table_collected}
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
#    0.900 0.910 0.920 0.930 0.940 \
#    0.950 0.960 0.970 0.980 0.990 \
#    0.991 0.992 0.993 0.994 0.995 \
#    0.996 0.997 0.998 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_collected} ${selected_file} 0 2 \
##    0.900 0.990 0.995 0.997 0.999

#done


set +x


