#!/usr/local/bin/zsh

cd /home/zpeng/pppp/clion/panns_mac/vtune

./PSS_v5_LG_degree_reorder /scratch/zpeng/deep1b/deep100M_base.fvecs /scratch/zpeng/deep1b/deep100M_query.fvecs /scratch/zpeng/deep1b/deep100M.nsg 100 output.ivces /scratch/zpeng/deep1b/deep100M.true-100_NN.v2.binary 16 216 216 1 0 0 0 216 216 1 /scratch/zpeng/deep1b/deep100M_reorder_map.expand.binary 100000