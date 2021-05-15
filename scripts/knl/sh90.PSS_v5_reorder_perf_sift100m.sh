#!/usr/local/bin/zsh

base_dir="/home/zpeng/data"
#base_dir="/scratch/zpeng"
perf_flags="stat -B -e cycles,instructions,page-faults,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads,LLC-store-misses,LLC-stores,dTLB-load-misses,dTLB-loads,dTLB-store-misses,dTLB-stores,iTLB-load-misses,iTLB-loads,cache-misses,cache-references"

data_dir="${base_dir}/sift1b"
data="sift100M"
opt_thrd=100000

set -x

#for reorder_mode in expand in_degree; do
reorder_mode="expand"
opt_thrd=350000

######### T1
#num_t=1
#L=1007
#X=999999
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######### T2
#num_t=2
#L=507
#X=515
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######### T4
#num_t=4
#L=257
#X=257
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######## T8
num_t=8
L=132
X=132
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
#
#
######### T16
#num_t=16
#L=108
#X=70
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######### T32
#num_t=32
#L=108
#X=39
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######### T64
#num_t=64
#L=100
#X=24
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#done

#for reorder_mode in expand in_degree; do
reorder_mode="in_degree"
opt_thrd=280000

######### T1
#num_t=1
#L=1007
#X=999999
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######### T2
#num_t=2
#L=507
#X=515
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######### T4
#num_t=4
#L=257
#X=257
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######## T8
num_t=8
L=132
X=132
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
#
#
######### T16
#num_t=16
#L=108
#X=70
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######### T32
#num_t=32
#L=108
#X=39
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
#
######### T64
#num_t=64
#L=100
#X=24
#bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 ${data_dir}/${data}_reorder_map.${reorder_mode}.binary ${opt_thrd}"
#
##### Whole Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="whole_search"
#app="./PSS_v5_LG_degree_reorder"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="reorder_${reorder_mode}_PSS"
#app_type="no_search"
#app="./PSS_v5_LG_degree_reorder_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

#done
set +x


