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

bin="./profile_find_X_para_single_query_search_simple_v3"

label=${tag}
:> output.${label}.raw.txt

P_dest=0.97
L=98
${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg ${L} 100 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 ${P_dest} | tee -a output.${label}.raw.txt

P_dest=0.98
L=99
${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg ${L} 100 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 8 ${P_dest} | tee -a output.${label}.raw.txt

python3 ../scripts/output_surrounding.py output.${label}.raw.txt output.${label}.row.txt;
python3 ../scripts/output_format.py output.${label}.row.txt output.${label}.table.txt 0:6;

