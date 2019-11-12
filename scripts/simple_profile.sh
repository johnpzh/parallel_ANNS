#! /bin/bash

# Do computation with trace so without graph traverse
bin=/home/zpeng/pppp/clion/searching_mac/cmake-build-release/tests/app_search_in_batch

# SIFT
data_path=/scratch/zpeng/sift1m
data_name=sift
k=200
l=200
output=output.${data_name}.txt
echo "---- ${data_path}/${data_name} ----"
${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee ${output}

# GIST
data_path=/scratch/zpeng/gist1m
data_name=gist
k=400
l=400
output=output.${data_name}.txt
echo "---- ${data_path}/${data_name} ----"
${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 1000 | tee ${output}

# DEEP10M
data_path=/scratch/zpeng/deep1b
data_name=deep10M
k=400
l=400
output=output.${data_name}.txt
echo "---- ${data_path}/${data_name} ----"
${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee ${output}

## Do computation with trace so without graph traverse
#bin=/home/zpeng/pppp/clion/searching_mac/cmake-build-release/tests/test_nsg_optimized_search
##bin=/home/zpeng/pppp/clion/searching_mac/cmake-build-release/tests/profile_search_to_record_trace
#bin_search=/home/zpeng/pppp/clion/searching_mac/cmake-build-release/tests/profile_search_on_recorded_trace
#
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee ${output}
#${bin_search} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.trace.q-10000.binary | tee -a ${output}
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 1000 | tee ${output}
#${bin_search} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 1000 ${data_path}/${data_name}.trace.q-1000.binary | tee -a ${output}
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee ${output}
#${bin_search} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.trace.q-10000.binary | tee -a ${output}

## Graph Degree Distribution
#bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_graph_degree_distribution
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original Graph" | tee ${output}
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}.nsg | tee -a ${output}
#echo "Reversed Graph" | tee -a ${output}
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}.reversed.nsg | tee -a ${output}
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original Graph" | tee ${output}
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}.nsg | tee -a ${output}
#echo "Reversed Graph" | tee -a ${output}
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}.reversed.nsg | tee -a ${output}
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original Graph" | tee ${output}
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}.nsg | tee -a ${output}
#echo "Reversed Graph" | tee -a ${output}
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}.reversed.nsg | tee -a ${output}


## Measure shared candidates
#bin=/home/zpeng/benchmarks/clion/nsg_mac/cmake-build-release/tests/profile_search_shared_tops
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k 40
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k 40
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k 40
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k 40
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k 40
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k 40

## Measure shared top-ranked
#bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_search_shared_tops
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k 10000
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k 10000
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k 1000
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k 1000
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k 10000
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k 10000

## Try Reversed NSG searching
#bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_precision
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=100
#l=100
##k=200
##l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.true-100_NN.q-10000.binary
#echo "Reversed_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.true-100_NN.q-10000.binary
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=100
#l=100
##k=400
##l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000  ${data_path}/${data_name}.true-100_NN.q-1000.binary
#echo "Reversed_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.true-100_NN.q-1000.binary
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=100
#l=100
##k=400
##l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.true-100_NN.q-10000.binary
#echo "Reversed_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.true-100_NN.q-10000.binary

## Generate Reversed NSG
#bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_reversed_nsg_index
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_${k}nn.graph 40 50 500 ${data_path}/${data_name}.reversed.nsg
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_${k}nn.graph 60 70 500 ${data_path}/${data_name}.reversed.nsg
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_${k}nn.graph 60 70 500 ${data_path}/${data_name}.reversed.nsg
#
## Try Reversed NSG searching
#bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/test_nsg_optimized_search
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000
#echo "Reversed_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k output.ivecs 10000
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000
#echo "Reversed_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k output.ivecs 10000
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "Original_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000
#echo "Reversed_NSG"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.reversed.nsg $l $k output.ivecs 10000

#
## Batched Joint Traversal Search, and Sequntial Search
#bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_precision
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.true-100_NN.q-10000.binary
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.true-100_NN.q-1000.binary
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 ${data_path}/${data_name}.true-100_NN.q-10000.binary

## Batched Joint Traversal Search, and Sequntial Search
#bin_joint_searching=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_batched_joint_traversal_search
#bin_normal_searching=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/test_nsg_optimized_search
### Breakdown optimized searching
##bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_breakdown_optimized_searching
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "---- Sequntial Search ----" | tee ${output}
#${bin_normal_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 10000 | tee -a ${output}
#echo "---- Batched Joint Search ----" | tee -a ${output}
#${bin_joint_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 10000 | tee -a ${output}
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "---- Sequntial Search ----" | tee ${output}
#${bin_normal_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 10000 | tee -a ${output}
#echo "---- Batched Joint Search ----" | tee -a ${output}
#${bin_joint_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 10000 | tee -a ${output}
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "---- Sequntial Search ----" | tee ${output}
#${bin_normal_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 10000 | tee -a ${output}
#echo "---- Batched Joint Search ----" | tee -a ${output}
#${bin_joint_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 10000 | tee -a ${output}

## Normal searching, Joint searching, and Parallel searching
## Generate paired queries
#bin_normal_searching=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/test_nsg_optimized_search
#bin_joint_searching=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_joint_traversal_search
#bin_parallel_searching=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_parallel_optimized_search
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "normal_searching:" > ${output}
#${bin_normal_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee -a ${output}
#echo "joint_searching" >> ${output}
#${bin_joint_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee -a ${output}
#echo "parallel_searching" >> ${output}
#${bin_parallel_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 32 | tee -a ${output}
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "normal_searching:" > ${output}
#${bin_normal_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee -a ${output}
#echo "joint_searching" >> ${output}
#${bin_joint_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee -a ${output}
#echo "parallel_searching" >> ${output}
#${bin_parallel_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 32 | tee -a ${output}
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#echo "normal_searching:" > ${output}
#${bin_normal_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee -a ${output}
#echo "joint_searching" >> ${output}
#${bin_joint_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 | tee -a ${output}
#echo "parallel_searching" >> ${output}
#${bin_parallel_searching} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_all_same_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs 10000 32 | tee -a ${output}

## Generate paired queries
#bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_pair_queries
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}_paired_query.fvecs 500
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}_paired_query.fvecs 500
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}_paired_query.fvecs 500
#
#
## Measure percentage of shared candidates
#bin=/home/zpeng/benchmarks/clion/nsg_th107b4/cmake-build-release/tests/profile_search_shared_candidates
#
## SIFT
#data_path=/scratch/zpeng/sift1m
#data_name=sift
#k=200
#l=200
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 20 | tee ${output}
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 20 | tee ${output}
#
## GIST
#data_path=/scratch/zpeng/gist1m
#data_name=gist
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 20 | tee ${output}
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 20 | tee ${output}
#
## DEEP10M
#data_path=/scratch/zpeng/deep1b
#data_name=deep10M
#k=400
#l=400
#output=output.${data_name}.txt
#echo "---- ${data_path}/${data_name} ----"
#${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 20 | tee ${output}
##${bin} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_paired_query.fvecs ${data_path}/${data_name}.nsg $l $k output.${data_name}.ivecs 20 | tee ${output}

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
#echo "---- ${data_path}/${data_name} ----"
##for ((vol = vol_start; vol <= vol_bound; vol += vol_step)); do
##	for ((dim = dim_start; dim <= dim_bound; dim += dim_step)); do
#for ((vol = vol_start; vol <= vol_bound; vol *= 10)); do
#	for ((dim = dim_start; dim <= dim_bound; dim *= 2)); do
#		./test_nsg_optimized_search ${data_path}/${data_name}_v${vol}d${dim}_base.fvecs ${data_path}/${data_name}_v${vol}d${dim}_query.fvecs ${data_path}/${data_name}_v${vol}d${dim}.nsg $L $K fake.ivecs 10000
#	done
#done

