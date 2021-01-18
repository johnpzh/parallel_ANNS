#!/usr/local/bin/zsh


set -x

data_dir="/home/zpeng/data"
data=deep10m

####################################
#### PSS
####################################
bin="python3 ../scripts/test63.find_L_top_m_scale_m.py"
app="profile_find_L_para_single_query_top_m_search_pure"
app_label="Top_M"
num_t=1
P_target=0.999
L_low=500
L_up=1000
for M in 002 004 008 016 032 064 128 256; do
    tag="${app_label}_T${num_t}_P${P_target}_M${M}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}
done

######
output_files="output.${data}.${app_label}_T${num_t}_P${P_target}_M"
table_file="output.${data}.${app_label}_T${num_t}_P${P_target}_M_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


