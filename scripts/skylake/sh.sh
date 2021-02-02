#scpt_path="../scripts/skylake"
#for i in {64..68}; do
#    zsh ./${scpt_path}/sh${i}*.sh
#done


data="sift1m"

######## X
#tag="test_X"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng ${data} ${tag} 16 104 240 240 20 10 50 5

###### L_local
tag="test_Local_L100"
python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng ${data} ${tag} 16 104 5 100 5 30 30 5
tag="test_Local_L4000"
python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng ${data} ${tag} 16 104 200 4000 300 30 30 5
input="output.${data}.test_Local_L"
output="output.${data}.test_Local_L_collect.txt"
cat ${input}*.table.txt > ${output}

######## L_master
#for L_master in 0100 0104 0108 0112 0116 0120 0124 0128 0132 0136 0140 0280 0560 1120 2240 4480 8960; do
#    tag="test_Master_L${L_master}"
#    python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng ${data} ${tag} 16 ${L_master} 240 240 10 5 5 5
#done
#input="output.${data}.test_Master_L"
#output="output.${data}.test_Master_L_collect.txt"
#cat ${input}*.table.txt > ${output}



#./profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 16 104 104 104 20 10 10 3
#./profile_find_L_para_single_query_search_PSS_v5_profile /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 16 104 104 104 10 0.999


#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng sift1m test 16 104 240 240 20 30 30 3

#python3 ../scripts/test49.find_L_PSS_v5_profile.py profile_find_L_para_single_query_search_PSS_v5_profile ~/data sift1m test 64 104 104 104 10 0.999

#python3 ../scripts/test49.find_L_PSS_v5_profile.py profile_find_L_para_single_query_search_PSS_v5_large_graph_profile /scratch/zpeng deep100m test 16 216 1 300 208 0.999


#scpt_path="../scripts/skylake"
##for i in {39..53}; do
##for i in {54..58}; do
##for i in {57..58}; do
##for i in {34..38}; do
##for i in {39..43}; do
#for i in {59..63}; do
#    zsh ./${scpt_path}/sh${i}*.sh
#done