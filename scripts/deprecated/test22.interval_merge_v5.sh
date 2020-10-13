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

#export KMP_AFFINITY="verbose,granularity=fine,compact,1,0"
#export KMP_AFFINITY="verbose,granularity=core,compact,1,0"
#export KMP_AFFINITY="verbose,granularity=fine,proclist=[0,4,8,6,2,12,16,18,14,10,20,24,28,26,22,32,36,38,34,30,1,5,9,7,3,13,17,19,15,11,21,25,29,27,23,33,37,39,35,31],explicit"
# For Pitzer
#export KMP_AFFINITY="granularity=fine,compact,0,0"
# For Skylake and KNL
export KMP_AFFINITY="granularity=core,compact,1,0"
## KNL and Pitzer are different

cd ../cmake-build-release || exit

bin="./profile_para_single_query_top_m_search_interval_merge.v5"

#:> output.${tag}.raw.txt
##for compt in 90000000 ; do
##for compt in 30000000 40000000 50000000 60000000 70000000 80000000 90000000; do
##for compt in 130000000 140000000 150000000 160000000 170000000 180000000 190000000; do
#for compt in 60000000 70000000 80000000 90000000 100000000 110000000 120000000; do
##    for sub_iter in 1 2; do
#    for ((sub_iter = 1; sub_iter <= 12; ++sub_iter)); do
#        ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 200 ${sub_iter} ${compt} | tee -a output.${tag}.raw.txt
##        ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 8 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 70 ${sub_iter} ${compt} | tee -a output.${tag}.raw.txt
##        ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 70 ${sub_iter} ${compt} > output.${sub_iter}.txt
#    done
#done
#
#python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 3 4 11 13 14 16 17;
#python3 ../scripts/output_row_minimum.py output.${tag}.row.txt output.${tag}.table.txt 2 0;

:> output.${tag}.raw.txt
#for compt in 190000000 ; do
#for compt in 30000000 40000000 50000000 60000000 70000000 80000000 90000000; do
#for compt in 130000000 140000000 150000000 160000000 170000000 180000000 190000000; do
#for compt in 60000000 70000000 80000000 90000000 100000000 110000000 120000000; do
#    for sub_iter in 1 2; do
for ((local_L = 200; local_L >= 1; local_L -= 10)); do
    for ((sub_iter = 1; sub_iter <= 12; ++sub_iter)); do
        ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 ${local_L} ${sub_iter} 190000000 | tee -a output.${tag}.raw.txt
#        ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 8 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 70 ${sub_iter} ${compt} | tee -a output.${tag}.raw.txt
#        ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs 128 ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 70 ${sub_iter} ${compt} > output.${sub_iter}.txt
    done
done

python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 3 4 11 13 14 16 17 18;
python3 ../scripts/output_row_minimum.py output.${tag}.row.txt output.${tag}.table.txt 2 0;


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