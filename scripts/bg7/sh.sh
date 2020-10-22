#!/home/zpeng/local/bin/zsh

cd /home/zpeng/pppp/clion/panns_mac/cmake-build-release

./profile_para_single_query_search_simple_v3_large_graph_ranged_with_metric /scratch/zpeng/data/sift1m/sift_base.fvecs /scratch/zpeng/data/sift1m/sift_query.fvecs /scratch/zpeng/data/sift1m/sift.nsg 100 100 /scratch/zpeng/data/sift1m/output.ivecs /scratch/zpeng/data/sift1m/sift.true-100_NN.v2.binary 8 100 1 90 1000
