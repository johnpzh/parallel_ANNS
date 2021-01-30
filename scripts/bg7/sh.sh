

./hnsw_search /scratch/zpeng/data/sift1b/sift100M.ef400.hnsw /scratch/zpeng/data/sift1b/sift100M_query.fvecs /scratch/zpeng/data/sift1b/sift100M.true-100_NN.v2.binary 100 128 10000 5000 10000 0.999

#
##scpt_path="../scripts/bg7"
##for i in {60..63}; do
##    zsh ./${scpt_path}/sh${i}*.sh
##done
#
#for ef in 200 1000 5000 10000 50000 100000; do
#    ./hnsw_search /scratch/zpeng/data/gist1m/gist.ef${ef}.hnsw /scratch/zpeng/data/gist1m/gist_query.fvecs /scratch/zpeng/data/gist1m/gist.true-100_NN.v2.binary 1 960 1000 5000 50000 0.999
#done
#
#output="output.txt"
#data_base="/scratch/zpeng/data"
#
#data="deep10M"
#query=
#for ef in 2000; do
#    index="${data_dir}/${data}.ef${ef}.hnsw"
#    ./hnsw_search ${data_dir}/deep
#done
#
##### DEEP10M
#data_dir="${data_base}/deep1b"
#data="deep10M"
##index="${data_dir}/${data}.hnsw"
#query="${data_dir}/${data}_query.fvecs"
#gt="${data_dir}/${data}.true-100_NN.v2.binary"
#output="output.search_${data}.txt"
#num_v=10
#dim=96
#num_q=10000
#${bin} ${index} ${query} ${gt} ${num_v} ${dim} ${num_q} | tee ${output}
#
##### SIFT100M
#data_dir="${data_base}/sift1b"
#data="sift100M"
#index="${data_dir}/${data}.hnsw"
#query="${data_dir}/${data}_query.fvecs"
#gt="${data_dir}/${data}.true-100_NN.v2.binary"
#output="output.search_${data}.txt"
#num_v=100
#dim=128
#num_q=10000
#${bin} ${index} ${query} ${gt} ${num_v} ${dim} ${num_q} | tee ${output}
#
##### DEEP100M
#data_dir="${data_base}/deep1b"
#data="deep100M"
#index="${data_dir}/${data}.hnsw"
#query="${data_dir}/${data}_query.fvecs"
#gt="${data_dir}/${data}.true-100_NN.v2.binary"
#output="output.search_${data}.txt"
#num_v=100
#dim=96
#num_q=10000
#${bin} ${index} ${query} ${gt} ${num_v} ${dim} ${num_q} | tee ${output}