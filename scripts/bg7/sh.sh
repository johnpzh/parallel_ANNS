#!/home/zpeng/local/bin/zsh

cd /home/zpeng/pppp/clion/panns_mac/cmake-build-release
#data_path="/scratch/zpeng/data/deep1b"
#data_name="deep10M"
data_path="/scratch/zpeng/data/sift1m"
data_name="sift"

./profile_para_single_query_search_simple_v3_large_graph_ranged_with_metric_mean ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg 100 100 ${data_path}/output.ivecs ${data_path}/${data_name}.true-100_NN.v2.binary 2 100 1 2 1000
