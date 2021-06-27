#!/usr/local/bin/zsh

set -x

data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
data=sift100m

###############
#### SGS
###############
bin="python3 ../scripts/test48.find_L_simple_search.py"
app="NSG_seq_LG_percentile"
app_label="NSG_SEQ_percentile"
#num_t=16

#P_target=0.900
#tag="${app_label}_find_L_P${P_target}"
#L_low=90
#L_up=200
#P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.990
#tag="${app_label}_find_L_P${P_target}"
#L_low=200
#L_up=300
#P_ts="0.99 0.991 0.992 0.993 0.994 0.995"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.996
#tag="${app_label}_find_L_P${P_target}"
#L_low=300
#L_up=400
#P_ts="0.996 0.997"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.999
tag="${app_label}_find_L_P${P_target}"
L_low=1030
L_up=1050
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

######
output_files="output.${data}.${app_label}_find_L_P0.9"
table_file="output.${data}.${app_label}_find_L_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


