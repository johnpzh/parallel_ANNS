#!/usr/local/bin/zsh

#python3 ../scripts/test67.PSS_v5_count_reorder_ranged_L.py PSS_v5_LG_degree_reorder /scratch/zpeng deep100m deep100m.degree_reorder 16 208 224 8 0 0 0 208 224 8 100000
##
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py ./profile_para_single_query_search_PSS_v4_large_graph_ranged_profiling /scratch/zpeng deep100m deep100m.v4 16 208 224 8 208 224 8
#
#python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py ./PSS_v5_LG_distance_threshold_profiling /scratch/zpeng/ deep100m deep100m.v5 16 208 224 8 0 0 0 208 224 8 0 0 0

#./PSS_v5_LG_v_counter /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 99999 99999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary
#
#./PSS_v5_LG_v_counter /scratch/zpeng/sift1b/sift100M_base.fvecs /scratch/zpeng/sift1b/sift100M_query.fvecs /scratch/zpeng/sift1b/sift100M.nsg 100 output.ivecs /scratch/zpeng/sift1b/sift100M.true-100_NN.v2.binary 1 999 999 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1b/sift100M_reorder_map.binary

#./PSS_v5_LG_degree_reorder /scratch/zpeng/deep1b/deep10M_base.fvecs /scratch/zpeng/deep1b/deep10M_query.fvecs /scratch/zpeng/deep1b/deep10M.nsg 100 output.ivecs /scratch/zpeng/deep1b/deep10M.true-100_NN.v2.binary 1 890 890 1 0 0 0 999999 999999 1 /scratch/zpeng/deep1b/deep10M_reorder_map.fake.binary 0
#./PSS_v5_distance_threshold_profiling /scratch/zpeng/deep1b/deep10M_base.fvecs /scratch/zpeng/deep1b/deep10M_query.fvecs /scratch/zpeng/deep1b/deep10M.nsg 100 output.ivecs /scratch/zpeng/deep1b/deep10M.true-100_NN.v2.binary 1 890 890 1 0 0 0 999999 999999 1 0 0 0

#./PSS_v5_LG_degree_reorder /scratch/zpeng/deep1b/deep10M_base.fvecs /scratch/zpeng/deep1b/deep10M_query.fvecs /scratch/zpeng/deep1b/deep10M.nsg 100 output.ivecs /scratch/zpeng/deep1b/deep10M.true-100_NN.v2.binary 1 890 890 1 0 0 0 999999 999999 1 /scratch/zpeng/deep1b/deep10M_reorder_map.binary 1000000


#./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.fake.binary 0
##./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.fake.binary 1000000
./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 1000000
##./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 0
#./PSS_v5_distance_threshold_profiling /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999999 999999 1 0 0 0

#./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 16 104 104 1 0 0 0 30 30 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 1000000
#./PSS_v5_LG_degree_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 16 104 104 1 0 0 0 30 30 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 0
#./PSS_v5_distance_threshold_profiling /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 16 104 104 1 0 0 0 30 30 1 0 0 0