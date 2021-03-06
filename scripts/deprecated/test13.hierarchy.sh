#! /usr/bin/bash
####! /users/PGS0218/zpeng01/local/bin/zsh
####! /usr/local/bin/zsh
####! /bin/bash

#if [ $# -ne 1 ]; then
#    echo "Usage: $0 <data_directory>"
#    exit
#fi
#
#data_dir=$1

#set -x
## OpenMP Affinity for ICC

#export KMP_AFFINITY="verbose,granularity=fine,compact,1,0"
#export KMP_AFFINITY="verbose,granularity=core,compact,1,0"
#export KMP_AFFINITY="verbose,granularity=fine,proclist=[0,4,8,6,2,12,16,18,14,10,20,24,28,26,22,32,36,38,34,30,1,5,9,7,3,13,17,19,15,11,21,25,29,27,23,33,37,39,35,31],explicit"
#export KMP_AFFINITY="granularity=fine,compact,0,0"
export KMP_AFFINITY="granularity=core,compact,1,0"
## KNL and Pitzer are different

cd ../cmake-build-release || exit

numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 200 200 13
numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 200 200 10
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 200 200 7
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 200 200 5
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 100 100 5
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 50 50 5
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 40 40 5
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 30 40 5

#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 200 200 3
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 100 100 3
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 50 50 3
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 30 50 3
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 30 30 3

#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 200 200 4
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 100 100 4
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 50 50 4
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 30 50 4
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 30 30 4

#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 20 30 4
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 20 20 4
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 10 20 4
#numactl -m 0 ./profile_para_single_query_top_m_search_hierarchy_merge /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs 128 /scratch/zpeng/sift1m/sift.true-100_NN.q-10000.binary 8 8 10 10 4
