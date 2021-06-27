#!/usr/local/bin/zsh
set -x

#data_dir="/home/zpeng/data"
data_dir="/scratch/zpeng"
K=100
app_tag="Faiss_GPU"
bin="./4-GPU"

######################
##### SIFT1M
######################
#data="sift1m"
#data_name="sift"
#output_tag="${app_tag}_${data_name}"
#raw_file="output.${output_tag}.raw.txt"
#table_file="output.${output_tag}.table.txt"
#
##./4-GPU /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 100
#eval ${bin} ${data_dir}/${data}/${data_name}_base.fvecs ${data_dir}/${data}/${data_name}_query.fvecs ${data_dir}/${data}/${data_name}.true-100_NN.v2.binary ${K} 2>&1 | tee ${raw_file}
#python3 ../../scripts/output_rows_to_table.py ${raw_file} ${table_file} 0 1 2 3
#
######################
##### GIST1M
######################
#data="gist1m"
#data_name="gist"
#output_tag="${app_tag}_${data_name}"
#raw_file="output.${output_tag}.raw.txt"
#table_file="output.${output_tag}.table.txt"
#
#eval ${bin} ${data_dir}/${data}/${data_name}_base.fvecs ${data_dir}/${data}/${data_name}_query.fvecs ${data_dir}/${data}/${data_name}.true-100_NN.v2.binary ${K} 2>&1 | tee ${raw_file}
#python3 ../../scripts/output_rows_to_table.py ${raw_file} ${table_file} 0 1 2 3

#####################
#### DEEP10M
#####################
data="deep1b"
data_name="deep10M"
output_tag="${app_tag}_${data_name}"
raw_file="output.${output_tag}.raw.txt"
table_file="output.${output_tag}.table.txt"

eval ${bin} ${data_dir}/${data}/${data_name}_base.fvecs ${data_dir}/${data}/${data_name}_query.fvecs ${data_dir}/${data}/${data_name}.true-100_NN.v2.binary ${K} 2>&1 | tee ${raw_file}
python3 ../../scripts/output_rows_to_table.py ${raw_file} ${table_file} 0 1 2 3

#####################
#### SIFT100M
#####################
data="sift1b"
data_name="sift100M"
output_tag="${app_tag}_${data_name}"
raw_file="output.${output_tag}.raw.txt"
table_file="output.${output_tag}.table.txt"

eval ${bin} ${data_dir}/${data}/${data_name}_base.fvecs ${data_dir}/${data}/${data_name}_query.fvecs ${data_dir}/${data}/${data_name}.true-100_NN.v2.binary ${K} 2>&1 | tee ${raw_file}
python3 ../../scripts/output_rows_to_table.py ${raw_file} ${table_file} 0 1 2 3

#####################
#### DEEP100M
#####################
data="deep1b"
data_name="deep100M"
output_tag="${app_tag}_${data_name}"
raw_file="output.${output_tag}.raw.txt"
table_file="output.${output_tag}.table.txt"

eval ${bin} ${data_dir}/${data}/${data_name}_base.fvecs ${data_dir}/${data}/${data_name}_query.fvecs ${data_dir}/${data}/${data_name}.true-100_NN.v2.binary ${K} 2>&1 | tee ${raw_file}
python3 ../../scripts/output_rows_to_table.py ${raw_file} ${table_file} 0 1 2 3

set +x


