#!/usr/local/bin/zsh

set -x
#data_dir="/scratch/zpeng/data"
data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"


####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test70.app_para_searching.py"
app="app_para_searching_LG"
app_tag="baseline_multi_queries"

P_target=0.999

################################
#### SIFT100M
################################
data=sift100m
L=1039
#for num_t in 8 16 32 64; do
for num_t in 1 2 4; do
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done

################################
#### DEEP100M
################################
data=deep100m
L=4399
#for num_t in 8 16 32 64; do
for num_t in 1 2 4; do
    tag="${app_tag}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L}
    table_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.table.txt"
    selected_file="output.${data}.${app_tag}_T${num_t}_P${P_target}.selected.txt"
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
done

set +x


