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

bin="./profile_seq_single_query_top_m_search_scale_m"

:> output.${tag}.raw.txt
M=8
#for ((mid_iter = 0; mid_iter < 12; ++mid_iter)); do
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs ${M} ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary ${mid_iter} | tee -a output.${tag}.raw.txt
#done
${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs ${M} ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary | tee -a output.${tag}.raw.txt

python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 2:4 10 12:226;
#python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 2:4 10 12:29;
python3 ../scripts/output_row_minimum.py output.${tag}.row.txt output.${tag}.table.txt 2 0;
