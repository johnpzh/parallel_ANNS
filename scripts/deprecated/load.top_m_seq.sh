#!/bin/bash
#PBS -l nodes=1:bora:ppn=20
#PBS -l walltime=01:00:00
#PBS -N top_m_seq
#PBS -m abe
#PBS -o output.top_m_seq.txt
#PBS -e error.top_m_seq.txt
####PBS -j oe

# cd /sciclone/home20/zpeng01/pppp/clion/panns_mac/cmake-build-release || exit
## ./profile_para_single_query_top_m_search_better_merge /sciclone/pscr/zpeng01/sift1m/sift_base.fvecs /sciclone/pscr/zpeng01/sift1m/sift_query.fvecs /sciclone/pscr/zpeng01/sift1m/sift.nsg 200 200 output.ivecs 128 /sciclone/pscr/zpeng01/sift1m/sift.true-100_NN.q-10000.binary 2
# ./profile_para_single_query_top_m_search_better_merge /sciclone/pscr/zpeng01/sift1m/sift_base.fvecs /sciclone/pscr/zpeng01/sift1m/sift_query.fvecs /sciclone/pscr/zpeng01/sift1m/sift.nsg 200 200 output.ivecs 128 /sciclone/pscr/zpeng01/sift1m/sift.true-100_NN.q-10000.binary 1

cd /sciclone/home20/zpeng01/pppp/clion/panns_mac/cmake-build-release || exit
######### top_m_seq_search
bin_panns=./app_top_m_seq_search
#num_t_max=1
#num_t_max=1
value_m=128

#
## SIFT
data_path=/sciclone/pscr/zpeng01/sift1m
data_name=sift
k=200
l=200
echo "----${data_name}----"
${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary

## GIST
data_path=/sciclone/pscr/zpeng01/gist1m
data_name=gist
k=400
l=400
echo "----${data_name}----"
${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-1000.binary

## DEEP10M
data_path=/sciclone/pscr/zpeng01/deep1b
data_name=deep10M
k=400
l=400
echo "----${data_name}----"
${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${value_m} ${data_path}/${data_name}.true-100_NN.q-10000.binary

######### simple_search
bin_panns=./app_seq_searching
#num_t_max=1
#num_t_max=1
value_m=128

#
## SIFT
data_path=/sciclone/pscr/zpeng01/sift1m
data_name=sift
k=200
l=298
echo "----${data_name}----"
${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${data_path}/${data_name}.true-100_NN.q-10000.binary

## GIST
data_path=/sciclone/pscr/zpeng01/gist1m
data_name=gist
k=400
l=477
echo "----${data_name}----"
${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${data_path}/${data_name}.true-100_NN.q-1000.binary

## DEEP10M
data_path=/sciclone/pscr/zpeng01/deep1b
data_name=deep10M
k=400
l=489
echo "----${data_name}----"
${bin_panns} ${data_path}/${data_name}_base.fvecs ${data_path}/${data_name}_query.fvecs ${data_path}/${data_name}.nsg $l $k output.ivecs ${data_path}/${data_name}.true-100_NN.q-10000.binary


# qsub -I -l nodes=2:c11a:ppn=8 -l walltime=08:00:00 -X

#module add intel/2019

#cd /sciclone/home20/zpeng01/pppp/graphPhi/tools/vertex_id_reorder_to_degrees
#./friendster.sh

#cd /sciclone/home20/zpeng01/pppp/graphPhi/tools/tiling_process
#./test.sh


#cd /sciclone/home20/zpeng01/Download/mvapich2-2.3.1
#make -j && make install

#cd /sciclone/home20/zpeng01/Download/boost_1_69_0
#./b2 install threading=multi

#cd /sciclone/home20/zpeng01/Download
##tar xzf gcc-7.4.0.tar.gz
##cd gcc-7.4.0
##./contrib/download_prerequisites
##cd ..
#mkdir build_gcc740
#cd build_gcc740
#$PWD/../gcc-7.4.0/configure --prefix=/sciclone/home20/zpeng01/lib/gcc-7.4.0 \
			#--with-system-zlib \
			#--enable-shared \
			#--enable-threads=posix \
			#--enable-multilib
#make && make install

#module add boost/1.69.0-gcc-7.4.0 mvapich2/2.3.1-gcc-7.4.0
#cd /sciclone/home20/zpeng01/pppp/Galois/build/dist_apps
#mvp2run -a -c 2 -m blocked ./bfs_push ../inputs/reference/scalefree/rmat8-2e14.gr -runs=10
##mvp2run -c 2 -m blocked ./bfs_push ../inputs/reference/scalefree/rmat8-2e14.gr -runs=10

#module add mvapich2/2.2/intel-hfi
#cd /sciclone/home20/zpeng01/pppp/pado/cmake-build-release/distributed_version
#set np = 1
#while ( $np <= 8 )
#	mvp2run -a -c ${np} -m blocked ./dpado /sciclone/scr-mlt/zpeng01/hollywood/hollywood.binary >&output_holl_${np}.txt
#	@ np = ${np} * 2
#end

#path=/sciclone/pscr/zpeng01
## Download
#cd /sciclone/home20/zpeng01/Download
#./get_datasets.sh ${path}

## Binary
#cd /sciclone/home20/zpeng01/pppp/pado/cmake-build-release/distributed_version/tools/graph-convert
#./edgelist2binary ${path}/indochina/indochina.txt ${path}/indochina/indochina.binary
#./edgelist2binary ${path}/friendster/friendster.txt ${path}/friendster/friendster.binary
