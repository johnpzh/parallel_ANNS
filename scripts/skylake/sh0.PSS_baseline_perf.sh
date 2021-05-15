#!/usr/local/bin/zsh

#./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.fake.binary 0
##./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.fake.binary 1000000
#./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 1000000
##./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 0
./PSS_v5_distance_threshold_profiling /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 0 0 0

#./PSS_v5_sequence_traverse /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 0 0 0 7464
