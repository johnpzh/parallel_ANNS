#!/usr/local/bin/zsh

base_dir="/home/zpeng/data"
#base_dir="/scratch/zpeng"
perf_flags="stat -B -e cycles,instructions,page-faults,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads,LLC-store-misses,LLC-stores,dTLB-load-misses,dTLB-loads,dTLB-store-misses,dTLB-stores,iTLB-load-misses,iTLB-loads,cache-misses,cache-references"

data_dir="${base_dir}/sift1m"
data="sift"
opt_thrd=1000000

set -x

for reorder_mode in expand in_degree; do

######## T1
num_t=1
L=413
X=999999
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"

#### Whole Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="whole_search"
app="./PSS_v5_LG_degree_reorder"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#### No Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="no_search"
app="./PSS_v5_LG_degree_reorder_no_search"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T2
num_t=2
L=214
X=222
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"

#### Whole Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="whole_search"
app="./PSS_v5_LG_degree_reorder"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#### No Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="no_search"
app="./PSS_v5_LG_degree_reorder_no_search"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T4
num_t=4
L=107
X=107
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"

#### Whole Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="whole_search"
app="./PSS_v5_LG_degree_reorder"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#### No Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="no_search"
app="./PSS_v5_LG_degree_reorder_no_search"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T8
num_t=8
L=100
X=53
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"

#### Whole Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="whole_search"
app="./PSS_v5_LG_degree_reorder"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#### No Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="no_search"
app="./PSS_v5_LG_degree_reorder_no_search"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T16
num_t=16
L=100
X=30
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"

#### Whole Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="whole_search"
app="./PSS_v5_LG_degree_reorder"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#### No Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="no_search"
app="./PSS_v5_LG_degree_reorder_no_search"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T32
num_t=32
L=100
X=17
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"

#### Whole Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="whole_search"
app="./PSS_v5_LG_degree_reorder"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#### No Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="no_search"
app="./PSS_v5_LG_degree_reorder_no_search"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T64
num_t=64
L=100
X=10
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"

#### Whole Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="whole_search"
app="./PSS_v5_LG_degree_reorder"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#### No Search
app_tag="reorder_${reorder_mode}_PSS"
app_type="no_search"
app="./PSS_v5_LG_degree_reorder_no_search"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

done
set +x


