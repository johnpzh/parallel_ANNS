#!/usr/local/bin/zsh

./PSS_v5_sequence_traverse_B /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 0 7464

./PSS_v5_sequence_traverse_B /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 412 413 1 0 0 0 99999 99999 1 0 0 0 /scratch/zpeng/sift1m/sift_reorder_map.binary 0 7464
