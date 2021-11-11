#!/usr/local/bin/zsh

if [[ "$#" -ne 2 ]]; then
    echo "Usage: $(basename $0) <data_parent_dir> <data_name>"
    exit
elif [[ "$2" != "sift1b" ]]; then
    echo "Error: the data_name should be sift1b not $2 ."
    exit
fi

set -x

#data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
#data_dir="/scratch/zpeng/data"
#data=deep1b
data_dir="$1"
data="$2"

###############
#### SGS
###############
bin="python3 ../scripts/test48.find_L_simple_search.py"
app="profile_find_L_seq_single_query_simple_search_large_graph"
app_label="NSG_SEQ"
#num_t=16

#P_target=0.900
#tag="${app_label}_find_L_P${P_target}"
#L_low=15900
#L_up=16200
#P_ts="0.9"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.990
tag="${app_label}_find_L_P${P_target}"
L_low=26200
L_up=80000
P_ts="0.950"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

#######
#output_files="output.${data}.${app_label}_find_L_P0.9"
#table_file="output.${data}.${app_label}_find_L_collected.table.txt"
#cat ${output_files}*.table.txt > ${table_file}

set +x


