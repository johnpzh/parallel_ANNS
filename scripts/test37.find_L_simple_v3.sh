#! /usr/bin/bash
####! /users/PGS0218/zpeng01/local/bin/zsh
####! /usr/local/bin/zsh
####! /bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <data_directory> <tag>"
    exit
fi

data_dir=$1
tag=$2

#set -x
## OpenMP Affinity for ICC
# For Pitzer
#export KMP_AFFINITY="granularity=fine,compact,0,0"
# For Skylake and KNL
export KMP_AFFINITY="granularity=core,compact,1,0"
## KNL and Pitzer are different

cd ../cmake-build-release || exit

bin="./profile_find_L_para_single_query_search_simple_v3"

label=${tag}
:> output.${label}.raw.txt
for P_dest in 0.90 0.91 0.92 0.93 0.94; do
    ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 87 100 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 0 8 100 ${P_dest} | tee -a output.${label}.raw.txt
done

#for P_dest in 0.95 0.96 0.97 0.98 0.99; do
#    ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 90 100 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 0 8 200 ${P_dest} | tee -a output.${label}.raw.txt
#done
#
#for P_dest in 0.991 0.992 0.993 0.994; do
#    ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 150 100 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 0 8 200 ${P_dest} | tee -a output.${label}.raw.txt
#done
#
#for P_dest in 0.995 0.996 0.997 0.998 0.999; do
#    ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 100 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 0 8 500 ${P_dest} | tee -a output.${label}.raw.txt
#done

python3 ../scripts/output_surrounding.py output.${label}.raw.txt output.${label}.row.txt;
python3 ../scripts/output_format.py output.${label}.row.txt output.${label}.table.txt 0:6;

#python3 ../scripts/output_format.py output.${label}.raw.txt output.${label}.row.txt 2 3 10 12 13 15;
#python3 ../scripts/output_row_minimum.py output.${label}.row.txt output.${label}.table.txt 2 0;


#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 16 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 200 1 200000000

#bin="./profile_para_single_query_top_m_search_simple_top_m.v2"
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 200 4 200000000 > output.0.txt

#bin="./profile_para_single_query_top_m_search_m_width.v2"
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 200 4 200000000 1 > output.txt


#:> output.${tag}.raw.txt
##for compt in 90000000 ; do
##for compt in 30000000 40000000 50000000 60000000 70000000 80000000 90000000; do
##for compt in 130000000 140000000 150000000 160000000 170000000 180000000 190000000; do
##for compt in 60000000 70000000 80000000 90000000 100000000 110000000 120000000; do
##    for sub_iter in 1 2; do
##    for ((sub_iter = 1; sub_iter <= 12; ++sub_iter)); do
#    for ((num_t = 1; num_t <= 64; num_t *= 2)); do
#        ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary ${num_t} 200 1 200000000 | tee -a output.${tag}.raw.txt
#    done
##done
#
#python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 3 4 11 13 14 16 17;
#python3 ../scripts/output_row_minimum.py output.${tag}.row.txt output.${tag}.table.txt 2 0;

#
#:> output.${tag}.raw.txt
### SIFT1M
#data_path=${data_dir}/sift1m
#data_name=sift
#echo "----${data_name}----" | tee -a output.${tag}.raw.txt
#for ((num_t = 1; num_t <= 64; num_t *= 2)); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg 200 200 output.ivecs 128 ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} 200 1 200000000 | tee -a output.${tag}.raw.txt
#done
#
### GIST1M
#data_path=${data_dir}/gist1m
#data_name=gist
#echo "----${data_name}----" | tee -a output.${tag}.raw.txt
#for ((num_t = 1; num_t <= 64; num_t *= 2)); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg 200 200 output.ivecs 128 ${data_path}/${data_name}.true-100_NN.q-1000.binary ${num_t} 200 1 200000000 | tee -a output.${tag}.raw.txt
#done
#
### DEEP10M
#data_path=${data_dir}/deep1b
#data_name=deep10M
#echo "----${data_name}----" | tee -a output.${tag}.raw.txt
#for ((num_t = 1; num_t <= 64; num_t *= 2)); do
#    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg 200 200 output.ivecs 128 ${data_path}/${data_name}.true-100_NN.q-10000.binary ${num_t} 200 1 200000000 | tee -a output.${tag}.raw.txt
#done
#
#python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 3 4 11 13 14 16 17;
#python3 ../scripts/output_row_minimum.py output.${tag}.row.txt output.${tag}.table.txt 2 0;

#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 4 200 1 90000000
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 70 1 90000000 > output.1.txt
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 70 2 90000000 > output.2.txt
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 70 1 190000000 > output.3.txt


#for compt in 200000000 190000000 180000000 170000000 160000000 150000000; do
#    ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 200 1 ${compt}
##${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 70 1 150000000
#done

#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 1 90000000 > output.1.txt
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 2 90000000 > output.2.txt

#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8
#numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge.v3 ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 1 4 70 1 90000000
#numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge.v3 ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 1 90000000
#diff output.0.txt output.txt



#numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge.v3 ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 1 90000000 > output.1.txt
#numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge.v3 ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 2 90000000 > output.2.txt
#numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge.v3 ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 3 90000000 > output.3.txt
#numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge.v3 ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 4 90000000 > output.4.txt


#numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge.v3 ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 2 20000000




#perf stat -B -e L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,LLC-load-misses,LLC-loads,cache-misses,cache-references,task-clock,context-switches,cpu-migrations,page-faults,cycles,instructions,branches,branch-misses numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 1 20000000
#
#perf stat -B -e L1-dcache-load-misses,L1-dcache-loads,L1-icache-load-misses,LLC-load-misses,LLC-loads,cache-misses,cache-references,task-clock,context-switches,cpu-migrations,page-faults,cycles,instructions,branches,branch-misses numactl -m 0 ./profile_para_single_query_top_m_search_interval_merge ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 8 70 2 20000000