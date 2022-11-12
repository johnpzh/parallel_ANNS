set -x

## SIFT1M
data=sift1m
output_tag="NSG_seq_bdw_${data}"
pcm-memory.x -- ./profile_find_L_seq_single_query_simple_search /home/zpeng/data/sift1m/sift_base.fvecs /home/zpeng/data/sift1m/sift_query.fvecs /home/zpeng/data/sift1m/sift.nsg 427 100 output.ivecs /home/zpeng/data/sift1m/sift.true-100_NN.v2.binary 427 0.999 2>&1 | tee output.${output_tag}.txt

## GIST1M
data=gist1m
output_tag="NSG_seq_bdw_${data}"
pcm-memory.x -- ./profile_find_L_seq_single_query_simple_search /home/zpeng/data/gist1m/gist_base.fvecs /home/zpeng/data/gist1m/gist_query.fvecs /home/zpeng/data/gist1m/gist.nsg 2280 100 output.ivecs /home/zpeng/data/gist1m/gist.true-100_NN.v2.binary 2280 0.999 2>&1 | tee output.${output_tag}.txt

## DEEP10M
data=deep10m
output_tag="NSG_seq_bdw_${data}"
pcm-memory.x -- ./profile_find_L_seq_single_query_simple_search /home/zpeng/data/deep1b/deep10M_base.fvecs /home/zpeng/data/deep1b/deep10M_query.fvecs /home/zpeng/data/deep1b/deep10M.nsg 994 100 output.ivecs /home/zpeng/data/deep1b/deep10M.true-100_NN.v2.binary 994 0.999 2>&1 | tee output.${output_tag}.txt

## SIFT100M
data=sift100m
output_tag="NSG_seq_bdw_${data}"
pcm-memory.x -- ./profile_find_L_seq_single_query_simple_search /home/zpeng/data/sift1b/sift100M_base.fvecs /home/zpeng/data/sift1b/sift100M_query.fvecs /home/zpeng/data/sift1b/sift100M.nsg 1039 100 output.ivecs /home/zpeng/data/sift1b/sift100M.true-100_NN.v2.binary 1039 0.999 2>&1 | tee output.${output_tag}.txt

## DEEP100M
data=deep100m
output_tag="NSG_seq_bdw_${data}"
pcm-memory.x -- ./profile_find_L_seq_single_query_simple_search /home/zpeng/data/deep1b/deep100M_base.fvecs /home/zpeng/data/deep1b/deep100M_query.fvecs /home/zpeng/data/deep1b/deep100M.nsg 4399 100 output.ivecs /home/zpeng/data/deep1b/deep100M.true-100_NN.v2.binary 4399 0.999 2>&1 | tee output.${output_tag}.txt


set +x