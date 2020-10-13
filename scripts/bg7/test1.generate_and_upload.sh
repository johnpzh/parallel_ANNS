#!/bin/bash

dir=/scratch/zpeng/data/sift1b
name=sift100M
base_file=${dir}/${name}_base.fvecs
nn_file=${dir}/${name}_200nn.graph
nsg_file=${dir}/${name}.nsg
query_file=${dir}/${name}_query.fvecs
true_nn_file=${dir}/${name}.true-100_NN.v2.binary

set -x
## SIFT100M_base file
#./widget_get_sift100M /scratch/zpeng/data/sift1b/bigann_base.bvecs 100000000 ${base_file}
#
## kNN file
#cd ../../efanna_graph_mac/cmake-build-release/tests
#./test_nndescent ${base_file} ${nn_file} 200 200 12 20 100
#
## NSG file
#cd ../../../panns_mac/cmake-build-release
#./test_nsg_index ${base_file} ${nn_file} 60 70 500 ${nsg_file}
#
## Query file
#./convert_bvecs_to_fvecs ${dir}/bigann_query.bvecs ${query_file}

# scp
for file in ${base_file} ${nn_file} ${nsg_file} ${query_file} ${true_nn_file}; do
	#scp ${file} skylake:/scratch/zpeng/sift1b/
	#scp ${file} knl:/data/zpeng/sift1b/
	scp ${file} zpeng01@bora.sciclone.wm.edu:/sciclone/pscr/zpeng01/sift1b/
done

set +x





#base_dir=/scratch/zpeng/data
#
#data_dir=${base_dir}/sift1m
#data_name=sift
#./check_true_nn ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.true-100_NN.v2.binary 100 64
#
#data_dir=${base_dir}/gist1m
#data_name=gist
#./check_true_nn ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.true-100_NN.v2.binary 100 64
#
#data_dir=${base_dir}/deep1b
#data_name=deep10M
#./check_true_nn ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.true-100_NN.v2.binary 100 64
#
#data_dir=${base_dir}/deep1b
#data_name=deep100M
#./check_true_nn ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.true-100_NN.v2.binary 100 64

#./save_true_nearest_neighbors /scratch/zpeng/data/deep1b/deep10M_base.fvecs /scratch/zpeng/data/deep1b/deep10M_query.fvecs 100 /scratch/zpeng/data/deep1b/deep10M.true-100_NN.v2.binary 64
#./save_true_nearest_neighbors /scratch/zpeng/data/sift1m/sift_base.fvecs /scratch/zpeng/data/sift1m/sift_query.fvecs 100 /scratch/zpeng/data/sift1m/sift.true-100_NN.v2.binary 64
#./save_true_nearest_neighbors /scratch/zpeng/data/gist1m/gist_base.fvecs /scratch/zpeng/data/gist1m/gist_query.fvecs 100 /scratch/zpeng/data/gist1m/gist.true-100_NN.v2.binary 64
#./save_true_nearest_neighbors /scratch/zpeng/data/deep1b/deep100M_base.fvecs /scratch/zpeng/data/deep1b/deep100M_query.fvecs 100 /scratch/zpeng/data/deep1b/deep100M.true-100_NN.v2.binary 64

#./test_nsg_index /scratch/zpeng/data/sift1b/sift100M_base.fvecs /scratch/zpeng/data/sift1b/sift100M_200nn.iter12.S20.graph 40 50 500 /scratch/zpeng/data/sift1b/sift100M.200nn.iter12.S20.L40.R50.nsg

#./test_nsg_index /scratch/zpeng/data/deep1b/deep100M_base.fvecs /scratch/zpeng/data/deep1b/deep100M_200nn.iter12.S15.graph 40 50 500 /scratch/zpeng/data/deep1b/deep100M.nsg
#./test_nsg_index /scratch/zpeng/data/deep1b/deep100M_base.fvecs /scratch/zpeng/data/deep1b/deep100M_400nn.iter12.S20.graph 60 70 500 /scratch/zpeng/data/deep1b/deep100M.400nn.iter12.S20.L60.R70.nsg
#./test_nsg_index /scratch/zpeng/data/deep1b/deep100M_base.fvecs /scratch/zpeng/data/deep1b/deep100M_400nn.iter12.S20.graph 200 70 500 /scratch/zpeng/data/deep1b/deep100M.400nn.iter12.S20.L200.R70.nsg
#./test_nsg_index /scratch/zpeng/data/deep1b/deep100M_base.fvecs /scratch/zpeng/data/deep1b/deep100M_400nn.iter12.S20.graph 400 70 500 /scratch/zpeng/data/deep1b/deep100M.400nn.iter12.S20.L400.R70.nsg
#./test_nsg_index /scratch/zpeng/data/deep1b/deep100M_base.fvecs /scratch/zpeng/data/deep1b/deep100M_400nn.iter12.S20.graph 800 70 500 /scratch/zpeng/data/deep1b/deep100M.400nn.iter12.S20.L800.R70.nsg

#L=200
#for iter in 12 14 16; do
#	for S in 15 18 20; do
#		echo "---- L:${L} iter:${iter} S:${S} ----"
#		./test_nndescent /scratch/zpeng/data/deep1b/deep100M_base.fvecs /scratch/zpeng/data/deep1b/deep100M_${L}nn.iter${iter}.S${S}.graph ${L} ${L} ${iter} ${S} 100
#	done
#done

#L=400
##iter=14
#S=20
#for iter in 14 16 18 20; do
#	echo "---- L:${L} iter:${iter} S:${S} ----"
#	./test_nndescent /scratch/zpeng/data/deep1b/deep100M_base.fvecs /scratch/zpeng/data/deep1b/deep100M_${L}nn.iter${iter}.S${S}.graph ${L} ${L} ${iter} ${S} 100
#done

#L=400
#iter=12
#S=20
#./test_nndescent /scratch/zpeng/data/deep1b/deep200M_base.fvecs /scratch/zpeng/data/deep1b/deep200M_${L}nn.iter${iter}.S${S}.graph ${L} ${L} ${iter} ${S} 100
