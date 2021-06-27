#!/usr/local/bin/zsh

base_dir="/home/zpeng/data"
#base_dir="/scratch/zpeng"
perf_flags="stat -B -e cycles,instructions,page-faults,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads,LLC-store-misses,LLC-stores,dTLB-load-misses,dTLB-loads,dTLB-store-misses,dTLB-stores,iTLB-load-misses,iTLB-loads,cache-misses,cache-references"

data_dir="${base_dir}/gist1m"
data="gist"

set -x

######## T1
num_t=1
L=2224
X=999999
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0"

#### Whole Search
app_tag="PSS_v5_perf"
app_type="whole_search"
app="./PSS_v5_distance_threshold_profiling"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}
#
##### No Search
#app_tag="PSS_v5_perf"
#app_type="no_search"
#app="./PSS_v5_distance_threshold_profiling_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

######## T2
num_t=2
L=1112
X=1128
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0"

#### Whole Search
app_tag="PSS_v5_perf"
app_type="whole_search"
app="./PSS_v5_distance_threshold_profiling"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

##### No Search
#app_tag="PSS_v5_perf"
#app_type="no_search"
#app="./PSS_v5_distance_threshold_profiling_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T4
num_t=4
L=564
X=572
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0"

#### Whole Search
app_tag="PSS_v5_perf"
app_type="whole_search"
app="./PSS_v5_distance_threshold_profiling"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

##### No Search
#app_tag="PSS_v5_perf"
#app_type="no_search"
#app="./PSS_v5_distance_threshold_profiling_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T8
num_t=8
L=278
X=294
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0"

#### Whole Search
app_tag="PSS_v5_perf"
app_type="whole_search"
app="./PSS_v5_distance_threshold_profiling"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

##### No Search
#app_tag="PSS_v5_perf"
#app_type="no_search"
#app="./PSS_v5_distance_threshold_profiling_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T16
num_t=16
L=147
X=147
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0"

#### Whole Search
app_tag="PSS_v5_perf"
app_type="whole_search"
app="./PSS_v5_distance_threshold_profiling"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

##### No Search
#app_tag="PSS_v5_perf"
#app_type="no_search"
#app="./PSS_v5_distance_threshold_profiling_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T32
num_t=32
L=108
X=77
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0"

#### Whole Search
app_tag="PSS_v5_perf"
app_type="whole_search"
app="./PSS_v5_distance_threshold_profiling"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

##### No Search
#app_tag="PSS_v5_perf"
#app_type="no_search"
#app="./PSS_v5_distance_threshold_profiling_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}


######## T64
num_t=64
L=108
X=42
bin_flags="${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivces ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0"

#### Whole Search
app_tag="PSS_v5_perf"
app_type="whole_search"
app="./PSS_v5_distance_threshold_profiling"
bin="numactl -m 0 ${app}"
output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"

eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

##### No Search
#app_tag="PSS_v5_perf"
#app_type="no_search"
#app="./PSS_v5_distance_threshold_profiling_no_search"
#bin="numactl -m 0 ${app}"
#output="output.${app_tag}.${data}.T${num_t}.${app_type}.txt"
#
#eval perf ${perf_flags} ${bin} ${bin_flags} 2>&1 | tee ${output}

set +x


