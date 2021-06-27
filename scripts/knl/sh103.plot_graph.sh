#!/usr/local/bin/zsh

base_dir="/home/zpeng/data"
q_id=17

#################################
#### SIFT1M
#################################
data_dir="${base_dir}/sift1m"
data="sift"


num_t=1
L=413
X=9999999
tag="${data}_Q${q_id}_T${num_t}_subgraph"
subgraph_file_0="output.${tag}.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph_file_0} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

subgraph_file_1="output.${tag}_con2nn.txt"
figure="output.${tag}_con2nn.png"
# python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.pdf
python ../tools/plot_graph.py ${subgraph_file_1} ${figure}


num_t=2
L=214
X=214
tag="${data}_Q${q_id}_T${num_t}_subgraph"
subgraph_file_0="output.${tag}.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph_file_0} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

subgraph_file_1="output.${tag}_con2nn.txt"
figure="output.${tag}_con2nn.png"
# python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.pdf
python ../tools/plot_graph.py ${subgraph_file_1} ${figure}


num_t=4
L=107
X=107
tag="${data}_Q${q_id}_T${num_t}_subgraph"
subgraph_file_0="output.${tag}.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph_file_0} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

subgraph_file_1="output.${tag}_con2nn.txt"
figure="output.${tag}_con2nn.png"
# python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.pdf
python ../tools/plot_graph.py ${subgraph_file_1} ${figure}


num_t=8
L=100
X=57
tag="${data}_Q${q_id}_T${num_t}_subgraph"
subgraph_file_0="output.${tag}.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph_file_0} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

subgraph_file_1="output.${tag}_con2nn.txt"
figure="output.${tag}_con2nn.png"
# python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.pdf
python ../tools/plot_graph.py ${subgraph_file_1} ${figure}


num_t=16
L=100
X=30
tag="${data}_Q${q_id}_T${num_t}_subgraph"
subgraph_file_0="output.${tag}.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph_file_0} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

subgraph_file_1="output.${tag}_con2nn.txt"
figure="output.${tag}_con2nn.png"
# python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.pdf
python ../tools/plot_graph.py ${subgraph_file_1} ${figure}


num_t=32
L=100
X=17
tag="${data}_Q${q_id}_T${num_t}_subgraph"
subgraph_file_0="output.${tag}.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph_file_0} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

subgraph_file_1="output.${tag}_con2nn.txt"
figure="output.${tag}_con2nn.png"
# python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.pdf
python ../tools/plot_graph.py ${subgraph_file_1} ${figure}


num_t=64
L=104
X=10
tag="${data}_Q${q_id}_T${num_t}_subgraph"
subgraph_file_0="output.${tag}.txt"
./PSS_v5_LG_core_subgraph ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 ${subgraph_file_0} ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id}

subgraph_file_1="output.${tag}_con2nn.txt"
figure="output.${tag}_con2nn.png"
# python ../tools/plot_graph.py ${subgraph_file} output.sift_Q17_T1_subgraph.pdf
python ../tools/plot_graph.py ${subgraph_file_1} ${figure}