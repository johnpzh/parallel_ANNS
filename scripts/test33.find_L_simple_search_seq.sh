#! /usr/bin/bash
####! /users/PGS0218/zpeng01/local/bin/zsh
####! /usr/local/bin/zsh
####! /bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <data_directory> <tag>"
    exit
fi

base_dir=$1
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

#### SIFT1M
#data_dir="${base_dir}/sift1m"
#data_name="sift"
#label="${tag}.${data_name}"
#:> output.${label}.raw.txt
#for PE in 0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98 0.99 \
#            0.991 0.992 0.993 0.994 0.995 0.996 0.997 0.998 0.999; do
#    ${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg 85 0 output.ivecs ${data_dir}/${data_name}.true-100_NN.q-10000.binary 1000 ${PE} | tee -a output.${tag}.raw.txt
#done
#
#python3 ../scripts/output_surrounding.py output.${label}.raw.txt output.${label}.row.txt;
#python3 ../scripts/output_format.py output.${label}.row.txt output.${label}.table.txt 0:6;

### GIST1M
data_dir="${base_dir}/gist1m"
data_name="gist"
label="${tag}.${data_name}"
:> output.${label}.raw.txt
for PE in 0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98 0.99 \
            0.991 0.992 0.993 0.994 0.995 0.996 0.997 0.998 0.999; do
    ${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg 85 0 output.ivecs ${data_dir}/${data_name}.true-100_NN.q-1000.binary 1000 ${PE} | tee -a output.${tag}.raw.txt
done

python3 ../scripts/output_surrounding.py output.${label}.raw.txt output.${label}.row.txt;
python3 ../scripts/output_format.py output.${label}.row.txt output.${label}.table.txt 0:6;

### DEEP10M
data_dir="${base_dir}/deep1b"
data_name="deep10M"
label="${tag}.${data_name}"
:> output.${label}.raw.txt
for PE in 0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98 0.99 \
            0.991 0.992 0.993 0.994 0.995 0.996 0.997 0.998 0.999; do
    ${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg 85 0 output.ivecs ${data_dir}/${data_name}.true-100_NN.q-10000.binary 1000 ${PE} | tee -a output.${tag}.raw.txt
done

python3 ../scripts/output_surrounding.py output.${label}.raw.txt output.${label}.row.txt;
python3 ../scripts/output_format.py output.${label}.row.txt output.${label}.table.txt 0:6;

##########################
#for PE in  0.991 0.992 0.993 0.994; do
##for PE in 0.998 0.999; do
##for PE in 0.90 0.95 0.96 0.97 0.98 0.99 0.995; do
#    ${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 85 0 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 500 ${PE} | tee -a output.${tag}.raw.txt
#done
#
#python3 ../scripts/output_surrounding.py output.${label}.raw.txt output.${label}.row.txt;
#python3 ../scripts/output_format.py output.${label}.row.txt output.${label}.table.txt 0:6;

#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 300 0.995 | tee -a output.${tag}.raw.txt
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 230 0.994 | tee -a output.${tag}.raw.txt
#${bin} ${data_dir}/sift1m/sift_base.fvecs ${data_dir}/sift1m/sift_query.fvecs ${data_dir}/sift1m/sift.nsg 200 200 output.ivecs ${data_dir}/sift1m/sift.true-100_NN.q-10000.binary 230 0.993 | tee -a output.${tag}.raw.txt

#python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 1:3 9 11:225;
##python3 ../scripts/output_format.py output.${tag}.raw.txt output.${tag}.row.txt 2:4 10 12:29;
#python3 ../scripts/output_row_minimum.py output.${tag}.row.txt output.${tag}.table.txt 2 0;
