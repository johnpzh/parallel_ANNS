#!/usr/local/bin/zsh

dir_base="/scratch/zpeng"

#num_bins=1000
data_dir="${dir_base}/sift1m"
data="sift"
tag="${data}.dist_each_step"
L=428
echo -n "${tag} ... "
./NSG_candidate_distance_step ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg ${L} 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${L} 0.999 > output.${tag}.txt
echo "done."

data_dir="${dir_base}/gist1m"
data="gist"
tag="${data}.dist_each_step"
L=2300
echo -n "${tag} ... "
./NSG_candidate_distance_step ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg ${L} 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${L} 0.999 > output.${tag}.txt
echo "done."

data_dir="${dir_base}/deep1b"
data="deep10M"
tag="${data}.dist_each_step"
L=994
echo -n "${tag} ... "
./NSG_candidate_distance_step ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg ${L} 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${L} 0.999 > output.${tag}.txt
echo "done."

data_dir="${dir_base}/sift1b"
data="sift100M"
tag="${data}.dist_each_step"
L=1039
echo -n "${tag} ... "
./NSG_candidate_distance_step ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg ${L} 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${L} 0.999 > output.${tag}.txt
echo "done."

data_dir="${dir_base}/deep1b"
data="deep100M"
tag="${data}.dist_each_step"
L=4399
echo -n "${tag} ... "
./NSG_candidate_distance_step ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg ${L} 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${L} 0.999 > output.${tag}.txt
echo "done."

#./PSS_v5_LG_count_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999 999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 99999999
#./PSS_v5_LG_count_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999 999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 0
#./PSS_v5_LG_count_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 16 104 104 1 0 0 0 31 31 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 4000
#./PSS_v5_LG_count_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 16 104 104 1 0 0 0 31 31 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 999999999
#python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py ./PSS_v5_LG_distance_threshold_profiling /scratch/zpeng sift1m test 16 104 104 1 0 0 0 31 31 1 0 0 0
#./PSS_v5_LG_count_reorder /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 999 999 1 /scratch/zpeng/sift1m/sift_reorder_map.binary 4000
#python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py ./PSS_v5_LG_distance_threshold_profiling /scratch/zpeng sift1m test 1 413 413 1 0 0 0 999 999 1 0 0 0
#diff output.1.txt output.0.txt

#./PSS_v5_LG_v_counter /scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs /scratch/zpeng/sift1m/sift.nsg 100 output.ivecs /scratch/zpeng/sift1m/sift.true-100_NN.v2.binary 1 413 413 1 0 0 0 30 30 1 /scratch/zpeng/sift1m/sift_reorder_map.binary > output.txt

#./PSS_v5_LG_v_counter /scratch/zpeng/deep1b/deep100M_base.fvecs /scratch/zpeng/deep1b/deep100M_query.fvecs /scratch/zpeng/deep1b/deep100M.nsg 100 output.ivecs /scratch/zpeng/deep1b/deep100M.true-100_NN.v2.binary 1 3340 3340 1 0 0 0 99999 99999 1 /scratch/zpeng/deep1b/deep100M_reorder_map.binary > output.txt

#data_dir="/scratch/zpeng"
#num_t=16
#P_target=0.999
#
##### SIFT100M
#for M in 32 64 128 256; do
#    tag="test_M${M}"
#    data="sift100m"
#    L_low=800
#    L_up=1100
#    python3 ../scripts/test63.find_L_top_m_scale_m.py profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}
#
#    table_file="output.${data}.test_M_collected.table.txt"
#    cat output.${data}.test_M*.table.txt > ${table_file}
#done
#
##### DEEP100M
#for M in 32 64 128 256; do
#    tag="test_M${M}"
#    data="deep100m"
#    L_low=2000
#    L_up=4000
#    python3 ../scripts/test63.find_L_top_m_scale_m.py profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}
#
#    table_file="output.${data}.test_M_collected.table.txt"
#    cat output.${data}.test_M*.table.txt > ${table_file}
#done
#
##### SIFT1M
#for M in 32 64 128 256; do
#    tag="test_M${M}"
#    data="sift1m"
#    L_low=200
#    L_up=450
#    python3 ../scripts/test63.find_L_top_m_scale_m.py profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}
#
#    table_file="output.${data}.test_M_collected.table.txt"
#    cat output.${data}.test_M*.table.txt > ${table_file}
#done
#
##### GIST1M
#for M in 32 64 128 256; do
#    tag="test_M${M}"
#    data="gist1m"
#    L_low=2000
#    L_up=4000
#    python3 ../scripts/test63.find_L_top_m_scale_m.py profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}
#
#    table_file="output.${data}.test_M_collected.table.txt"
#    cat output.${data}.test_M*.table.txt > ${table_file}
#done
#
##### DEEP10M
#for M in 32 64 128 256; do
#    tag="test_M${M}"
#    data="deep10m"
#    L_low=700
#    L_up=1000
#    python3 ../scripts/test63.find_L_top_m_scale_m.py profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}
#
#    table_file="output.${data}.test_M_collected.table.txt"
#    cat output.${data}.test_M*.table.txt > ${table_file}
#done

#python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py PSS_v5_distance_threshold_profiling /scratch/zpeng sift1m test 16 100 100 1 0 0 0 30 30 1 0 0 0
##python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py PSS_v5_LG_distance_threshold_profiling /scratch/zpeng deep100m test 16 224 224 1 0 0 0 216 216 1 0 0 0
#
####### Some crazy stuff #######
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilinga /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilin /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefg /scratch/zpeng sift1m test 16 104 104 1 30 30 1
####### End #######


#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng sift1m test 16 104 104 1 0 0 0 30 30 1 0 0 0
#python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng sift1m test 16 100 100 1 0 0 0 30 30 1 0 0 0

#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v4_ranged_profiling /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py atest /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py btest /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilinga /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilin /scratch/zpeng sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng sift1m test 16 104 104 1 30 30 1

#cd /home/zpeng/pppp/clion/panns_mac/cmake-build-release
#data_dir="/scratch/zpeng"
#
#
#data="deep10m"
#num_t=16
#python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} test ${num_t} 120 120 1 240 240 1 62 64 1 107 107 1

#### DEEP10M v5_dt
#data="deep10m"
#num_t=16
#Master_L=1216
#Local_L=$((Master_L / num_t))
##python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} test ${num_t} ${Master_L} ${Master_L} 1 ${Local_L} ${Local_L} 1 1 40 1
##python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling ${data_dir} ${data} test ${num_t} ${Master_L} ${Master_L} 1 ${Local_L} ${Local_L} 1 1 40 1
##python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v6_ranged_profiling ${data_dir} ${data} test ${num_t} ${Master_L} ${Master_L} 1 ${Local_L} ${Local_L} 1 1 40 1
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v6_ranged_profiling ${data_dir} ${data} test ${num_t} 108 108 1 108 108 1 62 64 1 107 107 1

## SIFT1M v5_dt
#data="sift1m"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} test 16 101 101 1 300 300 1 29 31 1
## SIFT1M v5
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng sift1m test 16 101 101 1 300 300 1 29 31 1
## DEEP10M v5_dt
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng deep10m test 16 108 108 1 500 500 1 62 64 1
## DEEP10M v5
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng deep10m test 16 108 108 1 500 500 1 62 64 1

#Master_L=406
##Master_L=1600
#Local_L=$((Master_L / 16))
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng sift1m test 16 ${Master_L} ${Master_L} 1 ${Local_L} ${Local_L} 1 1 26 1
##python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng sift1m test 16 ${Master_L} ${Master_L} 1 ${Local_L} ${Local_L} 1 1 50 2

##########################################
######## DEEP10M PSS_v5
#data="deep10m"
#tag="PSS_v5_dt_Master_L"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} ${tag} 16 500 5000 500 1000 1000 10 63 63 5
##tag="PSS_v5_dt_X"
##python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} ${tag} 16 108 108 20 1000 1000 10 50 200 4
#tag="PSS_v5_dt_Local_L"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} ${tag} 16 108 108 20 10 100 10 63 63 5
################### End ###################

##########################################
######## SIFT1M PSS_v5
#data="sift1m"
#tag="PSS_v5_dt_Master_L"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} ${tag} 16 500 5000 500 1000 1000 10 30 30 5
##tag="PSS_v5_dt_X"
##python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} ${tag} 16 104 104 20 1000 1000 10 15 45 1
#tag="PSS_v5_dt_Local_L"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} ${tag} 16 108 108 20 10 90 10 30 30 5
################### End ###################


##################################
######## PSS_v5_dist_thresh
#data="sift1m"
#tag="PSS_v5_dt"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} ${tag} 16 100 1000 20 170 170 10 15 30 1
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999
#
######## PSS_v6
#tag="PSS_v6"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v6_ranged_profiling ${data_dir} ${data} ${tag} 16 100 1000 20 170 170 10 15 30 1
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999
############# End ###############


######## L&X
#tag="test_L_X"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng ${data} ${tag} 16 100 1000 50 1000 1000 20 25 25 5

######## X
#tag="test_X"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng ${data} ${tag} 16 104 240 240 20 10 50 5

####### L_local
#tag="test_Local_L100"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng ${data} ${tag} 16 104 5 100 5 30 30 5
#tag="test_Local_L4000"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling /scratch/zpeng ${data} ${tag} 16 104 200 4000 300 30 30 5
#input="output.${data}.test_Local_L"
#output="output.${data}.test_Local_L_collect.txt"
#cat ${input}*.table.txt > ${output}

######## L_master PSS_v6
#tag="test_Master_L"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v6_ranged_profiling /scratch/zpeng ${data} ${tag} 16 100 1000 50 1000 1000 10 25 25 5
####### L_master PSS_v5_dist_thresh
#tag="test_Master_L"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /scratch/zpeng ${data} ${tag} 16 100 1000 50 1000 1000 10 25 25 5


#for L_master in 0100 0104 0108 0112 0116 0120 0124 0128 0132 0136 0140 0280 0560 1120 2240 4480 8960; do
#    tag="test_Master_L${L_master}"
#    python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v6_ranged_profiling /scratch/zpeng ${data} ${tag} 16 ${L_master} 240 240 10 5 5 5
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

#scpt_path="../scripts/skylake"
###for ((i = 44; i <= 53; ++i)); do
###for ((i = 54; i <= 58; ++i)); do
#for ((i = 69; i <= 73; ++i)); do
#    zsh ./${scpt_path}/sh${i}*.sh
#done

