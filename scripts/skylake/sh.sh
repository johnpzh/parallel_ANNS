#!/usr/local/bin/zsh

#for num_t in 1 2 4 8 16 32 64; do
#    python ../tools/plot_graph.py output.sift_Q17_T${num_t}_subgraph.txt output.sift_Q17_T${num_t}_subgraph.png
#done
base_dir=
data_dir=
data=sift
q_id=17

num_t=1
L=
X=
tag=
subgraph_file="output.sift_Q17_T${num_t}_subgraph.txt"
./PSS_v5_LG_core_subgraph /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 ${subgraph_file} /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary ${num_t} 413 413 1 0 0 0 9999999 9999999 1 0 0 0 17

subgraph_file="output.sift_Q17_T${num_t}_subgraph_con2nn.txt"
# python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.pdf
 python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.png
