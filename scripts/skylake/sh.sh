#!/usr/local/bin/zsh
set -x
#data_dir="/scratch/zpeng/data"
#data_dir="/home/zpeng/data"
data_dir="/scratch/zpeng"
data=sift100m

####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py"
app="PSS_v5_multi_queries_LG"
app_tag="PSS_v5_multi_queries"
################################
#### Latency
################################

P_target=0.999
L_low=140
L_up=140
L_step=1
X_low=140
X_up=141
X_step=1

for num_t in 16; do
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done

set +x