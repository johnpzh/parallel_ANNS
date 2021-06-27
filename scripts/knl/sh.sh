#data_dir="/scratch/zpeng/deep1b"

base_dir="/home/zpeng/data"
q_id=17

#################################
#### DEEP100M
#################################
data_dir="${base_dir}/deep1b"
data="deep100M"

num_t=1
L=3340
X=9999999
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=2
L=1669
X=1685
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=4
L=834
X=842
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=8
L=417
X=433
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=16
L=208
X=208
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=32
L=104
X=112
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=64
L=100
X=68
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}