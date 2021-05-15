
data_base="/scratch/zpeng/data"
M=16

set -x

##### SIFT1M
#data_dir="${data_base}/sift1m"
#data="sift"
#input="${data_dir}/${data}_base.fvecs"
#output="${data_dir}/${data}.hnsw"
#num_v=1
#dim=128
#efConstruction=200
#./hnsw_index ${input} ${output} ${num_v} ${dim} ${M} ${efConstruction}

##### GIST1M
#data_dir="${data_base}/gist1m"
#data="gist"
#input="${data_dir}/${data}_base.fvecs"
#output="${data_dir}/${data}.hnsw"
##output="${data_dir}/${data}.hnsw"
#num_v=1
#dim=960
##efConstruction=200
#for efConstruction in 1000 5000 10000 50000 100000; do
#    output="${data_dir}/${data}.ef${efConstruction}.hnsw"
#    ./hnsw_index ${input} ${output} ${num_v} ${dim} ${M} ${efConstruction}
#done
#
##### DEEP10M
#data_dir="${data_base}/deep1b"
#data="deep10M"
#input="${data_dir}/${data}_base.fvecs"
#output="${data_dir}/${data}.hnsw"
#num_v=10
#dim=96
##efConstruction=200
#for efConstruction in 2000; do
#    output="${data_dir}/${data}.ef${efConstruction}.hnsw"
#    ./hnsw_index ${input} ${output} ${num_v} ${dim} ${M} ${efConstruction}
#done
#
##### SIFT100M
#data_dir="${data_base}/sift1b"
#data="sift100M"
#input="${data_dir}/${data}_base.fvecs"
#output="${data_dir}/${data}.hnsw"
#num_v=100
#dim=128
##efConstruction=40
#for efConstruction in 400 4000; do
#    output="${data_dir}/${data}.ef${efConstruction}.hnsw"
#    ./hnsw_index ${input} ${output} ${num_v} ${dim} ${M} ${efConstruction}
#done
#
##### DEEP100M
#data_dir="${data_base}/deep1b"
#data="deep100M"
#input="${data_dir}/${data}_base.fvecs"
#output="${data_dir}/${data}.hnsw"
#num_v=100
#dim=96
##efConstruction=40
#for efConstruction in 400 4000; do
#    output="${data_dir}/${data}.ef${efConstruction}.hnsw"
#    ./hnsw_index ${input} ${output} ${num_v} ${dim} ${M} ${efConstruction}
#done
#
##### SIFT100M
#data_dir="${data_base}/sift1b"
#data="sift100M"
#input="${data_dir}/${data}_base.fvecs"
#output="${data_dir}/${data}.hnsw"
#num_v=100
#dim=128
##efConstruction=40
#for efConstruction in 20000; do
#    output="${data_dir}/${data}.ef${efConstruction}.hnsw"
#    ./hnsw_index ${input} ${output} ${num_v} ${dim} ${M} ${efConstruction}
#done
#
##### DEEP100M
#data_dir="${data_base}/deep1b"
#data="deep100M"
#input="${data_dir}/${data}_base.fvecs"
#output="${data_dir}/${data}.hnsw"
#num_v=100
#dim=96
##efConstruction=40
#for efConstruction in 20000; do
#    output="${data_dir}/${data}.ef${efConstruction}.hnsw"
#    ./hnsw_index ${input} ${output} ${num_v} ${dim} ${M} ${efConstruction}
#done

#### DEEP1B
data_dir="${data_base}/deep1b"
data="deep1B"
input="${data_dir}/${data}_base.fvecs"
output="${data_dir}/${data}.hnsw"
num_v=1000
dim=96
#efConstruction=40
for efConstruction in 400; do
    output="${data_dir}/${data}.ef${efConstruction}.hnsw"
    ./hnsw_index ${input} ${output} ${num_v} ${dim} ${M} ${efConstruction}
done

set +x