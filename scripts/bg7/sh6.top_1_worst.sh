#!/home/zpeng/local/bin/zsh
set -x
bin=./profile_para_single_query_search_simple_v3_large_graph_worst_and_best_unchecked


##################################
## SIFT1M
##################################
base_dir=/scratch/zpeng/data
data_dir="${base_dir}/sift1m"
data_name=sift

#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
output="output.top_1_worst_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=${L_low}
L_step=1
X_low=60
X_up=$((X_low + 40))
X_step=10
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step} | tee ${output}
#
###################################
### GIST1M
###################################
#base_dir=/scratch/zpeng/data
#data_dir="${base_dir}/gist1m"
#data_name=gist
#
##### 8T
#num_t=8
####### P@100 0.90
#P_target=0.900
#output="output.top_1_worst_${data_name}_T${num_t}_${P_target}.txt"
#L_low=276
#L_up=${L_low}
#L_step=1
#X_low=210
#X_up=$((X_low + 40))
#X_step=10
#${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step} | tee ${output}

###################################
### DEEP10M
###################################
#base_dir=/scratch/zpeng/data
#data_dir="${base_dir}/deep1b"
#data_name=deep10M
#
##### 8T
#num_t=8
####### P@100 0.90
#P_target=0.900
#output="output.top_1_worst_${data_name}_T${num_t}_${P_target}.txt"
#L_low=114
#L_up=${L_low}
#L_step=1
#X_low=60
#X_up=$((X_low + 40))
#X_step=10
#${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step} | tee ${output}

###################################
### SIFT100M
###################################
#base_dir=/scratch/zpeng/data
#data_dir="${base_dir}/sift1b"
#data_name=sift100M
#
##### 8T
#num_t=8
####### P@100 0.90
#P_target=0.900
#output="output.top_1_worst_${data_name}_T${num_t}_${P_target}.txt"
#L_low=188
#L_up=${L_low}
#L_step=1
#X_low=110
#X_up=$((X_low + 40))
#X_step=10
#${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step} | tee ${output}
#
###################################
### DEEP100M
###################################
#base_dir=/scratch/zpeng/data
#data_dir="${base_dir}/deep1b"
#data_name=deep100M
#
##### 8T
#num_t=8
####### P@100 0.90
#P_target=0.900
#output="output.top_1_worst_${data_name}_T${num_t}_${P_target}.txt"
#L_low=448
#L_up=${L_low}
#L_step=1
#X_low=310
#X_up=$((X_low + 40))
#X_step=10
#${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step} | tee ${output}
