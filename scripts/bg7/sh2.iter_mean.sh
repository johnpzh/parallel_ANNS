#!/home/zpeng/local/bin/zsh

cd /home/zpeng/pppp/clion/panns_mac/cmake-build-release
#data_path="/scratch/zpeng/data/deep1b"
#data_name="deep10M"
data_path="/scratch/zpeng/data/sift1m"
data_name="sift"
l_lower=100
l_upper=100
l_step=1
num_t=8
x_start=50
x_step=1000

./profile_para_single_query_search_simple_v3_large_graph_ranged ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg ${l_lower} 100 ${data_path}/output.ivecs ${data_path}/${data_name}.true-100_NN.v2.binary ${num_t} ${l_upper} ${l_step} ${x_start} ${x_step}