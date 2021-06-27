#data_dir="/scratch/zpeng/deep1b"

base_dir="/scratch/zpeng/"
#base_dir="/home/zpeng/data"
q_id=17

#################################
#### SIFT1M
#################################
data_dir="${base_dir}/sift1m"
data="sift"

num_t=1
L=413
X=9999999
#subgraph="${data_dir}/${data}.source_subgraph_Q${q_id}.txt"
subgraph="output.${data}_Q${q_id}_T${num_t}_subgraph.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

num_t=2
L=214
X=214
subgraph="output.${data}_Q${q_id}_T${num_t}_subgraph.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

num_t=4
L=107
X=107
subgraph="output.${data}_Q${q_id}_T${num_t}_subgraph.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

num_t=8
L=100
X=57
subgraph="output.${data}_Q${q_id}_T${num_t}_subgraph.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

num_t=16
L=100
X=30
subgraph="output.${data}_Q${q_id}_T${num_t}_subgraph.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

num_t=32
L=100
X=17
subgraph="output.${data}_Q${q_id}_T${num_t}_subgraph.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

num_t=64
L=104
X=10
subgraph="output.${data}_Q${q_id}_T${num_t}_subgraph.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

##################################
##### GIST1M
##################################
#data_dir="${base_dir}/gist1m"
#data="gist"
#
#num_t=1
#L=2224
#X=999999
#subgraph="${data_dir}/${data}.source_subgraph_Q${q_id}.txt"
#./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
#
#
##################################
##### DEEP10M
##################################
#data_dir="${base_dir}/deep1b"
#data="deep10M"
#
#L=888
#X=9999999
#subgraph="${data_dir}/${data}.source_subgraph_Q${q_id}.txt"
#./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
#
##################################
##### SIFT100M
##################################
#data_dir="${base_dir}/sift1b"
#data="sift100M"
#
#num_t=1
#L=999
#X=9999999
#subgraph="${data_dir}/${data}.source_subgraph_Q${q_id}.txt"
#./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
#
#
##################################
##### DEEP100M
##################################
#data_dir="${base_dir}/deep1b"
#data="deep100M"
#
#num_t=1
#L=3340
#X=9999999
#subgraph="${data_dir}/${data}.source_subgraph_Q${q_id}.txt"
#./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}