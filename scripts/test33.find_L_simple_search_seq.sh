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

bin="./profile_find_L_seq_single_query_simple_search"

:> output.${tag}.raw.txt

for PE in 0.998 0.999; do
#for PE in 0.90 0.95 0.96 0.97 0.98 0.99 0.995; do
    ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 0 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 500 ${PE} | tee -a output.${tag}.raw.txt
done
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 300 0.995 | tee -a output.${tag}.raw.txt
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 230 0.994 | tee -a output.${tag}.raw.txt
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 230 0.993 | tee -a output.${tag}.raw.txt

#python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 1:3 9 11:225;
##python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 2:4 10 12:29;
#python3 ../scripts/output_row_minimum.py output.${tag}.row.txt output.${tag}.table.txt 2 0;
