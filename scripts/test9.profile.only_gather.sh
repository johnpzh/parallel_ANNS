#! /usr/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <data_directory>"
    exit
fi

data_dir=$1

#set -x
## OpenMP Affinity for ICC

#export KMP_AFFINITY="granularity=fine,compact,0,0"
export KMP_AFFINITY="granularity=core,compact,1,0"
## KNL and Pitzer are different

#cd ../cmake-build-debug || exit
cd ../cmake-build-release || exit

#data_dir=/users/PGS0218/zpeng01/data
#echo "-- Selecting --"
#python3 ../scripts/module_run.py ./profile_para_single_query_top_m_search_selecting_out.profile.seq_par.mth gist ~/data 256 ~/data/gist1m/gist.true-100_NN.q-1000.binary 4 400 4
#
#echo "-- Middle --"
#python3 ../scripts/module_run.py ./profile_para_single_query_top_m_search_middle_m.profile gist ~/data 256 ~/data/gist1m/gist.true-100_NN.q-1000.binary 4 4

#set +x

 #
    ## SIFT
    value_m=128
    data_path=${data_dir}/sift1m
    data_name=sift
    k=200
    l=200
    echo "----${data_name}----"
    echo "--Gather--"
    bin_panns=./profile_para_single_query_top_m_search_selecting_out.only_gather_top_m.profile
    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary 8 25 4

#    echo "--Middle--"
#    bin_panns=./profile_para_single_query_top_m_search_middle_m.profile
#    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary 8 4

#    ## GIST
#    value_m=256
#    data_path=${data_dir}/gist1m
#    data_name=gist
#    k=400
#    l=400
#    echo "----${data_name}----"
#
#echo "--Selecting--"
#bin_panns=./profile_para_single_query_top_m_search_selecting_out.profile.seq_par.mth
#    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary 2 200 4
#
#echo "--Middle--"
#bin_panns=./profile_para_single_query_top_m_search_middle_m.profile
#    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary 2 4

#    ## DEEP10M
#    value_m=256
#    data_path=${data_dir}/deep1b
#    data_name=deep10M
#    k=400
#    l=400
#    echo "----${data_name}----"
#    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary 1 400 1
#    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary 2 400 4
#    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary 4 200 8
#    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary 8 200 16
#    ${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary 16 50 16

#perf stat -e task-clock,cycles,instructions,cache-references,cache-misses ./profile_para_single_query_top_m_search_selecting_out.seq_par.mth ~/data/gist1m/gist_base.fvecs ~/data/gist1m/gist_query.fvecs ~/data/gist1m/gist.nsg 400 400 output.ivecs 256 ~/data/gist1m/gist.true-100_NN.q-1000.binary 4 400 4
##
#perf stat -e task-clock,cycles,instructions,cache-references,cache-misses ./profile_para_single_query_top_m_search_middle_m.profile ~/data/gist1m/gist_base.fvecs ~/data/gist1m/gist_query.fvecs ~/data/gist1m/gist.nsg 400 400 output.ivecs 256 ~/data/gist1m/gist.true-100_NN.q-1000.binary 4 4