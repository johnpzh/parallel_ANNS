#!/usr/local/bin/zsh

function get_L_low () {
    if (($1 < 10)); then
        echo $1
    elif (( ($1 / $2) < 10)); then
        echo 10
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

#data_dir="/scratch/zpeng/data"
data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
data=deep100m

####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test73.PSS_v5_K_values_ranged_L.py"
app="PSS_v5_LG_K_values"
#app="profile_para_single_query_search_PSS_v5_large_graph_dist_thresh_ranged_profiling"
#app_tag="PSS_v5_dt"

K=10
app_tag="PSS_v5_K_${K}"
################################
#### Latency
################################

#############
# 32 Threads
#############
#for num_t in 8; do
for num_t in 32; do

P_target=0.999
L_Seq=3011
L_low=58
L_up=58
L_step=16
X_low=50
X_up=58
X_step=4
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${K}
table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

P_target=0.995
L_Seq=601
L_low=34
L_up=34
L_step=8
X_low=19
X_up=25
X_step=3
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${K}
table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

P_target=0.990
L_Seq=355
L_low=27
L_up=27
L_step=8
X_low=20
X_up=26
X_step=3
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${K}
table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done

#############
# 1 Threads
#############
for num_t in 1; do

P_target=0.999
L_Seq=3011
L_low=$((L_Seq))
L_up=$((L_low + 2))
L_step=1
X_low=9999999
X_up=9999999
X_step=4
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${K}
table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

P_target=0.995
L_Seq=601
L_low=$((L_Seq))
L_up=$((L_low + 2))
L_step=1
X_low=9999999
X_up=9999999
X_step=4
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${K}
table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

P_target=0.990
L_Seq=355
L_low=$((L_Seq))
L_up=$((L_low + 2))
L_step=1
X_low=9999999
X_up=9999999
X_step=4
tag="${app_tag}_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${K}
table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done

set +x


