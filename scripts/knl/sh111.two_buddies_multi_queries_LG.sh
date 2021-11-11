#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
#data=sift100m

####################################
#### PSS Multiple Threads
####################################
bin_A="python3 ../scripts/test71.A.PSS_v5_dt_profiling_ranged_L.py"
bin_B="python3 ../scripts/test72.B.PSS_v5_dt_profiling_ranged_L.py"
app="PSS_v5_LG_distance_threshold_profiling"
#app_tag="PSS_v5_multi_queries"

################################
#### SIFT100M
################################
data=sift100m
P_target=0.999
L_low=140
L_up=140
L_step=1
X_low=138
X_up=141
X_step=1

for num_t in 8; do
#for num_t in 8 16 32 64; do
    app_tag_A="two_buddies_A"
    tag_A="${app_tag_A}_T${num_t}_P${P_target}"
    app_tag_B="two_buddies_B"
    tag_B="${app_tag_B}_T${num_t}_P${P_target}"

    eval ${bin_A} ${app} ${data_dir} ${data} ${tag_A} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 & \
    eval ${bin_B} ${app} ${data_dir} ${data} ${tag_B} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 &

    wait

    table_file_A="output.${data}.${app_tag_A}_T${num_t}_P${P_target}.table.txt"
    selected_file_A="output.${data}.${app_tag_A}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file_A} ${selected_file_A} 0 2 ${P_target}
    table_file_B="output.${data}.${app_tag_B}_T${num_t}_P${P_target}.table.txt"
    selected_file_B="output.${data}.${app_tag_B}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file_B} ${selected_file_B} 0 2 ${P_target}
done

################################
#### DEEP100M
################################
data=deep100m
P_target=0.999
L_low=417
L_up=417
L_step=1
X_low=431
X_up=434
X_step=1

for num_t in 8; do
#for num_t in 8 16 32 64; do
    app_tag_A="two_buddies_A"
    tag_A="${app_tag_A}_T${num_t}_P${P_target}"
    app_tag_B="two_buddies_B"
    tag_B="${app_tag_B}_T${num_t}_P${P_target}"

    eval ${bin_A} ${app} ${data_dir} ${data} ${tag_A} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 & \
    eval ${bin_B} ${app} ${data_dir} ${data} ${tag_B} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 &

    wait

    table_file_A="output.${data}.${app_tag_A}_T${num_t}_P${P_target}.table.txt"
    selected_file_A="output.${data}.${app_tag_A}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file_A} ${selected_file_A} 0 2 ${P_target}
    table_file_B="output.${data}.${app_tag_B}_T${num_t}_P${P_target}.table.txt"
    selected_file_B="output.${data}.${app_tag_B}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file_B} ${selected_file_B} 0 2 ${P_target}
done

set +x


