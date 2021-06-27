base_dir="/scratch/zpeng/data"
data_dir="${base_dir}/sift1m"
data="sift"
app_tag="parallel_search_path_subgraph"

#q_id=17

for q_id in 17 177; do

num_t=1
L=413
X=9999999
output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
./PSS_LG_parallel_search_path_in_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
python ../tools/plot_parallel_search_path_in_subgraph.py ${output_tag}.txt ${output_tag}.png ${data} ${q_id}

num_t=2
L=214
X=214
output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
./PSS_LG_parallel_search_path_in_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
python ../tools/plot_parallel_search_path_in_subgraph.py ${output_tag}.txt ${output_tag}.png ${data} ${q_id}

num_t=4
L=107
X=107
output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
./PSS_LG_parallel_search_path_in_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
python ../tools/plot_parallel_search_path_in_subgraph.py ${output_tag}.txt ${output_tag}.png ${data} ${q_id}

num_t=8
L=100
X=57
output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
./PSS_LG_parallel_search_path_in_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
python ../tools/plot_parallel_search_path_in_subgraph.py ${output_tag}.txt ${output_tag}.png ${data} ${q_id}

num_t=16
L=100
#L=500
X=30
#X=100
output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
./PSS_LG_parallel_search_path_in_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
python ../tools/plot_parallel_search_path_in_subgraph.py ${output_tag}.txt ${output_tag}.png ${data} ${q_id}

num_t=32
L=100
X=17
output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
./PSS_LG_parallel_search_path_in_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
python ../tools/plot_parallel_search_path_in_subgraph.py ${output_tag}.txt ${output_tag}.png ${data} ${q_id}

num_t=64
L=104
X=10
output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
./PSS_LG_parallel_search_path_in_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
python ../tools/plot_parallel_search_path_in_subgraph.py ${output_tag}.txt ${output_tag}.png ${data} ${q_id}

num_t=128
L=100
X=5
output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
./PSS_LG_parallel_search_path_in_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}
python ../tools/plot_parallel_search_path_in_subgraph.py ${output_tag}.txt ${output_tag}.png ${data} ${q_id}

done

###########################################
#for q_id in 7777; do
##for q_id in 17 71 77 117 177 1117 1177 7777; do
#    app_tag="mono_bfs_subgraph"
#    output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
#    ./PSS_LG_monotonic_bfs_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary 1 0 0 0 1 1 1 0 0 0 1 1 1 ${q_id}
#    python ../tools/plot_shortest_path_subfig.py ${output_tag}.txt ${output_tag}.png ${q_id}
#done
#
#for q_id in 7777; do
##for q_id in 17 71 77 117 177 1117 1177 7777; do
#    app_tag="all_mono_bfs_subgraph"
#    output_tag="output.${data}_${app_tag}_Q${q_id}_T${num_t}"
#    ./PSS_LG_all_monotonic_bfs_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${output_tag}.txt ${data_dir}/${data}.true-100_NN.v2.binary 1 0 0 0 1 1 1 0 0 0 1 1 1 ${q_id}
#    python ../tools/plot_all_monotonic_subfig.py ${output_tag}.txt ${output_tag}.png ${q_id}
#done


#
#base_dir="/scratch/zpeng/data"
#
#data_dir="${base_dir}/sift1m"
#data="sift"
#echo -n "${data} ... "
#./PSS_v5_LG_in_degree ${data_dir}/${data}_base.fvecs ${data_dir}/${data}.nsg ${data_dir}/${data}_reorder_map.in_degree.binary
#echo "done."
#
#data_dir="${base_dir}/sift1b"
#data="sift100M"
#echo -n "${data} ... "
#./PSS_v5_LG_in_degree ${data_dir}/${data}_base.fvecs ${data_dir}/${data}.nsg ${data_dir}/${data}_reorder_map.in_degree.binary
#echo "done."
#
#data_dir="${base_dir}/deep1b"
#data="deep100M"
#echo -n "${data} ... "
#./PSS_v5_LG_in_degree ${data_dir}/${data}_base.fvecs ${data_dir}/${data}.nsg ${data_dir}/${data}_reorder_map.in_degree.binary
#echo "done."