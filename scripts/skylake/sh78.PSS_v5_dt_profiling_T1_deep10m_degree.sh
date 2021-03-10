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

###################
##### PSS 1T
###################
bin="python3 ../scripts/test65.find_L_PSS_v4_profile_T1_degree.py"
app="profile_find_L_para_single_query_search_PSS_v4_profile"
app_tag="PSS_v5_dt"
num_t=1

####### R16
R=16
P_target=0.999
tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
L_low=20000
L_up=24950
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${R} ${P_ts}

######## R32
#R=32
#P_target=0.999
#tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#L_low=1200
#L_up=2000
#P_ts="0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${R} ${P_ts}

######## R64
#R=64
#P_target=0.999
#tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#L_low=790
#L_up=810
#P_ts="0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${R} ${P_ts}
##label="${data}_R${R}.${tag}"
##raw_file="output.${label}.raw.txt"
##:> ${raw_file}
###eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
##numactl -m 0 ./${app} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}_R${R}.nsg \
##${L_low} 100 output.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary \
##1 999999999 ${L_up} ${P_ts} \
##| tee -a ${raw_file}
##
##rows_file="output.${label}.rows.txt"
##table_file="output.${label}.table.txt"
##python3 ../scripts/output_surrounding.py ${raw_file} ${rows_file}
##python3 ../scripts/output_format.py ${rows_file} ${table_file} 0:16
#
######## R128
#R=128
#P_target=0.999
#tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#L_low=730
#L_up=735
#P_ts="0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${R} ${P_ts}
##label="${data}_R${R}.${tag}"
##raw_file="output.${label}.raw.txt"
##:> ${raw_file}
###eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
##numactl -m 0 ./${app} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}_R${R}.nsg \
##${L_low} 100 output.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary \
##1 999999999 ${L_up} ${P_ts} \
##| tee -a ${raw_file}
##
##rows_file="output.${label}.rows.txt"
##table_file="output.${label}.table.txt"
##python3 ../scripts/output_surrounding.py ${raw_file} ${rows_file}
##python3 ../scripts/output_format.py ${rows_file} ${table_file} 0:16
#
######## R256
#R=256
#P_target=0.999
#tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#L_low=730
#L_up=735
#P_ts="0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${R} ${P_ts}
##label="${data}_R${R}.${tag}"
##raw_file="output.${label}.raw.txt"
##:> ${raw_file}
###eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
##numactl -m 0 ./${app} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}_R${R}.nsg \
##${L_low} 100 output.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary \
##1 999999999 ${L_up} ${P_ts} \
##| tee -a ${raw_file}
##
##rows_file="output.${label}.rows.txt"
##table_file="output.${label}.table.txt"
##python3 ../scripts/output_surrounding.py ${raw_file} ${rows_file}
##python3 ../scripts/output_format.py ${rows_file} ${table_file} 0:16
#
######## R512
#R=512
#P_target=0.999
#tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#L_low=565
#L_up=570
#P_ts="0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${R} ${P_ts}
##label="${data}_R${R}.${tag}"
##raw_file="output.${label}.raw.txt"
##:> ${raw_file}
###eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
##numactl -m 0 ./${app} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}_R${R}.nsg \
##${L_low} 100 output.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary \
##1 999999999 ${L_up} ${P_ts} \
##| tee -a ${raw_file}
##
##rows_file="output.${label}.rows.txt"
##table_file="output.${label}.table.txt"
##python3 ../scripts/output_surrounding.py ${raw_file} ${rows_file}
##python3 ../scripts/output_format.py ${rows_file} ${table_file} 0:16
#
######## R1024
#R=1024
#P_target=0.999
#tag="${app_tag}_R${R}_T${num_t}_P${P_target}"
#L_low=495
#L_up=505
#P_ts="0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${R} ${P_ts}
##label="${data}_R${R}.${tag}"
##raw_file="output.${label}.raw.txt"
##:> ${raw_file}
###eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
##numactl -m 0 ./${app} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}_R${R}.nsg \
##${L_low} 100 output.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary \
##1 999999999 ${L_up} ${P_ts} \
##| tee -a ${raw_file}
##
##rows_file="output.${label}.rows.txt"
##table_file="output.${label}.table.txt"
##python3 ../scripts/output_surrounding.py ${raw_file} ${rows_file}
##python3 ../scripts/output_format.py ${rows_file} ${table_file} 0:16



set +x


