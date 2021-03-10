
#data_dir="/home/zpeng/data"
#data_dir="/scratch/zpeng"
#data_dir="/scratch/zpeng/data"

data_dir="/scratch/zpeng/data/deep1b"
data=deep10M

#for R in 70; do
#for R in 64 128 256 512 1024 2048; do
#    ./test_nsg_index ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_4096nn.graph 60 ${R} 500 ${data_dir}/${data}_R${R}.nsg
#for R in 512 1024 2048; do
#    ./test_nsg_index ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_4096nn.graph 60 ${R} $((R + 500)) ${data_dir}/${data}_R${R}.nsg
#done
for R in 32 16 8 4 2; do
    ./test_nsg_index ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_4096nn.graph 60 ${R} 500 ${data_dir}/${data}_R${R}.nsg
done

#data=deep1m

################
##### PSS
################
#bin="python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py"
#app="PSS_v5_distance_threshold_profiling"
#app_tag="PSS_v5_dt"
#
#
#
################
##### NSG
################
#bin="python3 ../scripts/test48.find_L_simple_search.py"
#app="profile_find_L_seq_single_query_simple_search"
#app_tag="NSG"
#
#P_target=0.900
#tag="${app_tag}_find_L_P${P_target}"
#L_low=90
#L_up=1000
#P_ts="0.9 0.91 0.92 0.93 0.94 \
#0.95 0.96 0.97 0.98 0.99 \
#0.991 0.992 0.993 0.994 0.995 \
#0.996 0.997 0.998 0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#######
#output_files="output.${data}.${app_tag}_find_L_P0.9"
#table_file="output.${data}.${app_tag}_find_L_collected.table.txt"
#cat ${output_files}*.table.txt > ${table_file}
#
#
################
##### HNSW
################
#bin="python3 ../scripts/test45.hnsw_find_ef.py"
#app="hnsw_search"
#app_tag="hnsw"
#
#P_target=0.900
#tag="${app_tag}_find_ef_P${P_target}"
#ef_low=1
#ef_up=2700
#P_ts="0.900 0.910 0.920 0.930 0.940 \
#0.950 0.960 0.970 0.980 0.990 \
#0.991 0.992 0.993 0.994 0.995 \
#0.996 0.997 0.998 0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}


#./hnsw_search /scratch/zpeng/data/sift1b/sift100M.ef400.hnsw /scratch/zpeng/data/sift1b/sift100M_query.fvecs /scratch/zpeng/data/sift1b/sift100M.true-100_NN.v2.binary 100 128 10000 5000 10000 0.999

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