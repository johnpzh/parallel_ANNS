#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
#data=sift100m

####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test80.PSS_v5_multi_queries_tuning_intra_ranged_L.py"
app="PSS_v5_multi_queries_tuning_intra_LG"
app_tag="${app}"

#################################
##### DEEP100M
#################################
data=deep100m
P_target=0.999
num_t=32

intra_num_t=32
L_low=104
L_up=104
L_step=1
X_low=112
X_up=115
X_step=1

tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
table_file="output.${data}.${tag}.table.txt"
selected_file="output.${data}.${tag}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#intra_num_t=16
#L_low=208
#L_up=208
#L_step=1
#X_low=208
#X_up=211
#X_step=1
#
#tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#intra_num_t=8
#L_low=417
#L_up=417
#L_step=1
#X_low=433
#X_up=436
#X_step=1
#
#tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#
#intra_num_t=4
#L_low=834
#L_up=834
#L_step=1
#X_low=842
#X_up=845
#X_step=1
#
#tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#
#intra_num_t=2
#L_low=1669
#L_up=1669
#L_step=1
#X_low=1685
#X_up=1688
#X_step=1
#
#tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

intra_num_t=1
L_low=3340
L_up=3343
L_step=1
X_low=999999
X_up=999999
X_step=1

tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
table_file="output.${data}.${tag}.table.txt"
selected_file="output.${data}.${tag}.selected.txt"
python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}


##################################
###### SIFT100M
##################################
#data=sift100m
#P_target=0.999
#num_t=32
#
#intra_num_t=16
#L_low=108
#L_up=108
#L_step=1
#X_low=78
#X_up=81
#X_step=1
#
#tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#intra_num_t=8
#L_low=140
#L_up=140
#L_step=1
#X_low=140
#X_up=143
#X_step=1
#
#tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#intra_num_t=4
#L_low=257
#L_up=257
#L_step=1
#X_low=265
#X_up=268
#X_step=1
#
#tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}
#
#intra_num_t=2
#L_low=507
#L_up=507
#L_step=1
#X_low=515
#X_up=518
#X_step=1
#
#tag="${app_tag}_inter$((num_t / intra_num_t))_inner$((intra_num_t))_P${P_target}"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} 0 0 0 ${X_low} ${X_up} ${X_step} 0 0 0 ${intra_num_t}
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 ${P_target}

set +x


