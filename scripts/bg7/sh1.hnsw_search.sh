
data_base="/scratch/zpeng/data"
bin="./hnsw_search"
set -x

##### SIFT1M
#data_dir="${data_base}/sift1m"
#data="sift"
#index="${data_dir}/${data}.hnsw"
#query="${data_dir}/${data}_query.fvecs"
#gt="${data_dir}/${data}.true-100_NN.v2.binary"
#output="output.search_${data}.txt"
#num_v=1
#dim=128
#num_q=10000
#${bin} ${index} ${query} ${gt} ${num_v} ${dim} ${num_q} | tee ${output}
#

##### GIST1M
#data_dir="${data_base}/gist1m"
#data="gist"
#efc=5000
#index="${data_dir}/${data}.ef${efc}.hnsw"
#query="${data_dir}/${data}_query.fvecs"
#gt="${data_dir}/${data}.true-100_NN.v2.binary"
#output="output.search_${data}.txt"
#num_v=1
#dim=960
#num_q=1000
#ef_low=10000
#ef_up=30000
#${bin} ${index} ${query} ${gt} ${num_v} ${dim} ${num_q} ${ef_low} ${ef_up} 0.999 | tee ${output}

##### DEEP10M
#data_dir="${data_base}/deep1b"
#data="deep10M"
#efc=2000
#index="${data_dir}/${data}.ef${efc}.hnsw"
#query="${data_dir}/${data}_query.fvecs"
#gt="${data_dir}/${data}.true-100_NN.v2.binary"
#output="output.search_${data}.txt"
#num_v=10
#dim=96
#num_q=10000
#ef_low=1000
#ef_up=10000
#${bin} ${index} ${query} ${gt} ${num_v} ${dim} ${num_q} ${ef_low} ${ef_up} 0.999 | tee ${output}
#
##### SIFT100M
#data_dir="${data_base}/sift1b"
#data="sift100M"
#efc=400
#index="${data_dir}/${data}.ef${efc}.hnsw"
#query="${data_dir}/${data}_query.fvecs"
#gt="${data_dir}/${data}.true-100_NN.v2.binary"
#output="output.search_${data}.txt"
#num_v=100
#dim=128
#num_q=10000
#ef_low=500
#ef_up=10000
#${bin} ${index} ${query} ${gt} ${num_v} ${dim} ${num_q} ${ef_low} ${ef_up} 0.999 | tee ${output}
#
#### DEEP100M
data_dir="${data_base}/deep1b"
data="deep100M"
efc=400
index="${data_dir}/${data}.ef${efc}.hnsw"
query="${data_dir}/${data}_query.fvecs"
gt="${data_dir}/${data}.true-100_NN.v2.binary"
output="output.search_${data}.txt"
num_v=100
dim=96
num_q=10000
ef_low=1000
ef_up=10000
${bin} ${index} ${query} ${gt} ${num_v} ${dim} ${num_q} ${ef_low} ${ef_up} 0.999 | tee ${output}

set +x