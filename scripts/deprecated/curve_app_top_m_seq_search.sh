#! /usr/local/bin/zsh
####! /bin/bash

cd ../cmake-build-release || exit
#####################
## Top-M Seq Search
#####################
bin=./profile_top_m_seq_search_scale_m
#bin=./app_top_m_seq_search
value_m=128

## SIFT
value_m=128
data_path=/data/zpeng/sift1m
#data_path=/scratch/zpeng/sift1m
data_name=sift
echo "----${data_name}----"
k=100
l_start=$k
l_bound=400
l_step=30
for (( l = l_start; l < l_bound; l += l_step )); do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary
done

l_start=$l_bound
l_bound=2000
l_step=200
for (( l = l_start; l < l_bound; l += l_step )); do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary
done

for l in 2000 4000 8000; do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary
done

## GIST
value_m=256
data_path=/data/zpeng/gist1m
#data_path=/scratch/zpeng/gist1m
data_name=gist
echo "----${data_name}----"
k=100
l_start=$k
l_bound=400
l_step=30
for (( l = l_start; l < l_bound; l += l_step )); do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary
done

l_start=$l_bound
l_bound=2000
l_step=200
for (( l = l_start; l < l_bound; l += l_step )); do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary
done

for l in 2000 4000 8000; do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary
done

## DEEP10M
value_m=256
data_path=/data/zpeng/deep1b
#data_path=/scratch/zpeng/deep1b
data_name=deep10M
echo "----${data_name}----"
k=100
l_start=$k
l_bound=400
l_step=30
for (( l = l_start; l < l_bound; l += l_step )); do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary
done

l_start=$l_bound
l_bound=2000
l_step=200
for (( l = l_start; l < l_bound; l += l_step )); do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary
done

for l in 2000 4000 8000; do
    ${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary
done


######################
### Top-M Seq Search
######################
#bin=./app_top_m_seq_search
#value_m=128
#
## SIFT
#data_path=/data/zpeng/sift1m
##data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#echo "====${data_name}===="
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary
#
## GIST
#data_path=/data/zpeng/gist1m
##data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#echo "====${data_name}===="
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary
#
## DEEP10M
#data_path=/data/zpeng/deep1b
##data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#echo "====${data_name}===="
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary

## FAKE
#data_path=/scratch/zpeng/fake
#data_name=fake
#vol_start=1000
#vol_bound=10000000
##vol_step=20000
#dim_start=64
#dim_bound=512
##dim_step=64
#K=400
#L=400
#echo "==== ${data_path}/${data_name} ===="
##for ((vol = vol_start; vol <= vol_bound; vol += vol_step)); do
##	for ((dim = dim_start; dim <= dim_bound; dim += dim_step)); do
#for ((vol = vol_start; vol <= vol_bound; vol *= 10)); do
#	for ((dim = dim_start; dim <= dim_bound; dim *= 2)); do
#		./test_nsg_optimized_search ${data_path}/${data_name}_v${vol}d${dim}_base.fvecs ${data_path}/${data_name}_v${vol}d${dim}_query.fvecs ${data_path}/${data_name}_v${vol}d${dim}.nsg $L $K fake.ivecs 10000
#	done
#done

