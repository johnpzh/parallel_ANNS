scpt_path="../scripts/knl"
for i in {64..68}; do
    zsh ./${scpt_path}/sh${i}*.sh
done

#python3 ../scripts/test49.find_L_PSS_v5_profile.py profile_find_L_para_single_query_search_PSS_v5_profile ~/data sift1m test 64 104 1 104 10 0.999

#./profile_para_single_query_search_PSS_v5_ranged_profiling ~/data/sift1m/sift_base.fvecs ~/data/sift1m/sift_query.fvecs ~/data/sift1m/sift.nsg 100 output.ivecs ~/data/sift1m/sift.true-100_NN.v2.binary 64 104 104 104 20 10 10 3
#./profile_find_L_para_single_query_search_PSS_v5_profile ~/data/sift1m/sift_base.fvecs ~/data/sift1m/sift_query.fvecs ~/data/sift1m/sift.nsg 100 output.ivecs ~/data/sift1m/sift.true-100_NN.v2.binary 64 104 104 104 10 0.999

#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling ~/data sift1m test 64 104 104 104 20 10 10 3
#python3 ../scripts/test49.find_L_PSS_v5_profile.py profile_find_L_para_single_query_search_PSS_v5_profile ~/data sift1m test 64 104 104 104 10 0.999

#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling ~/data gist1m test 64 108 108 108 20 42 42 3
#python3 ../scripts/test49.find_L_PSS_v5_profile.py profile_find_L_para_single_query_search_PSS_v5_profile ~/data gist1m test 64 108 108 108 42 0.999



##!/usr/local/bin/zsh
#scpt_path="../scripts/knl"
##for i in {39..53}; do
##for i in {54..58}; do
##for i in {39..43}; do
#for i in {59..63}; do
#    zsh ./${scpt_path}/sh${i}*.sh
#done