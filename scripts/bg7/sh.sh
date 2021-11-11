#!/usr/local/bin/zsh

data_dir="/scratch/zpeng/data"
app_tag="worker_deviation"
num_t=32
set -x
##################
#base_dir="sift1m"
#data="sift"
#L=100
#X=999999999
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=100
#X=17
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=100
#X=8
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}


###################
base_dir="deep1b"
data="deep100M"
#L=104
#X=999999999
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=104
#X=112
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=104
#X=56
#output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}

L=104
for X in 112; do
#for X in 999999999 112 56 28 14 7 3; do
    output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"

    ./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
done


###################
#base_dir="sift1b"
#data="sift100M"
##L=100
##X=999999999
##output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
##
##./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
##
##L=100
##X=39
##output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
##
##./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
##
##L=100
##X=19
##output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
##
##./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#
#L=100
#for X in 9 4 2; do
#    output="output.${app_tag}.${data}.T${num_t}.L${L}.X${X}.txt"
#
#    ./PSS_v5_worker_variation_LG ${data_dir}/${base_dir}/${data}_base.fvecs ${data_dir}/${base_dir}/${data}_query.fvecs ${data_dir}/${base_dir}/${data}.nsg 100 output.ivcs ${data_dir}/${base_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 2>&1 | tee ${output}
#done




set +x


