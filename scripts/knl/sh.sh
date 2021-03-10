
#for data in sift1m gist1m deep10m sift100m deep100m; do
#    python3 ../scripts/output_find_runtime_above_presicion.py \
#    output.${data}.PSS_v5_dt_T32_collected.table.txt \
#    output.${data}.PSS_v5_dt_T32_collected.selected_1.txt \
#    0 2 \
#    0.900 0.910 0.920 0.930 0.940 \
#    0.950 0.960 0.970 0.980 0.990 \
#    0.991 0.992 0.993 0.994 0.995 \
#    0.996 0.997 0.998 0.999
#done

#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilinga /home/zpeng/data sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling /home/zpeng/data sift1m test 16 104 104 1 30 30 1
#python3 ../scripts/test44.PSS_v4_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profilin /home/zpeng/data sift1m test 16 104 104 1 30 30 1

#data_dir="/home/zpeng/data"
#data=deep10m
#
### DEEP10M v5_dt
#num_t=64
##master_L_low=256
##master_L_up=256
##local_L_low=$((master_L_low))
##local_L_up=$((master_L_low))
##I_thresh_low=$((master_L_low - 1))
##I_thresh_up=$((I_thresh_low))
##python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} test ${num_t} ${master_L_low} ${master_L_up} 1 ${local_L_low} ${local_L_up} 1 20 20 1 ${I_thresh_low} ${I_thresh_up} 1
#python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} test ${num_t} 108 108 1 108 108 1 20 23 1 107 107 1
##python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} test ${num_t} 108 108 1 108 108 1 20 20 1 0 111 1
##python3 ../scripts/test51.PSS_v5_dt_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} test ${num_t} 108 108 1 108 108 1 19 21 1 99
##python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} test ${num_t} ${Master_L} ${Master_L} 1 ${Local_L} ${Local_L} 1 1 40 1
##python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v6_ranged_profiling /home/zpeng/data deep10m test ${num_t} ${Master_L} ${Master_L} 1 ${Local_L} ${Local_L} 1 1 40 1

#data="sift1m"

######## PSS_v5_dist_thresh
#tag="PSS_v5_dt"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_dist_thresh_ranged_profiling ${data_dir} ${data} ${tag} 64 100 300 20 500 500 10 7 13 1
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999
#
######## PSS_v6
#tag="PSS_v6"
#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v6_ranged_profiling ${data_dir} ${data} ${tag} 64 100 300 20 500 500 10 7 13 1
#table_file="output.${data}.${tag}.table.txt"
#selected_file="output.${data}.${tag}.selected.txt"
#python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 0.999

#scpt_path="../scripts/knl"
#for i in {64..68}; do
#    zsh ./${scpt_path}/sh${i}*.sh
#done

#python3 ../scripts/test49.find_L_PSS_v5_profile.py profile_find_L_para_single_query_search_PSS_v5_profile ~/data sift1m test 64 104 1 104 10 0.999

#./profile_para_single_query_search_PSS_v5_ranged_profiling ~/data/sift1m/sift_base.fvecs ~/data/sift1m/sift_query.fvecs ~/data/sift1m/sift.nsg 100 output.ivecs ~/data/sift1m/sift.true-100_NN.v2.binary 64 104 104 104 20 10 10 3
#./profile_find_L_para_single_query_search_PSS_v5_profile ~/data/sift1m/sift_base.fvecs ~/data/sift1m/sift_query.fvecs ~/data/sift1m/sift.nsg 100 output.ivecs ~/data/sift1m/sift.true-100_NN.v2.binary 64 104 104 104 10 0.999

#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling ~/data sift1m test 64 104 104 104 20 10 10 3
#python3 ../scripts/test49.find_L_PSS_v5_profile.py profile_find_L_para_single_query_search_PSS_v5_profile ~/data sift1m test 64 104 104 104 10 0.999

#python3 ../scripts/test47.PSS_v5_profiling_ranged_L.py profile_para_single_query_search_PSS_v5_ranged_profiling ~/data gist1m test 64 108 108 108 20 42 42 3
#python3 ../scripts/test49.find_L_PSS_v5_profile.py profile_find_L_para_single_query_search_PSS_v5_profile ~/data gist1m test 64 108 108 108 42 0.999


##
scpt_path="../scripts/knl"
for ((i = 44; i <= 46; ++i)); do
    zsh ./${scpt_path}/sh${i}*.sh
done
for ((i = 49; i <= 51; ++i)); do
    zsh ./${scpt_path}/sh${i}*.sh
done
#for ((i = 44; i <= 58; ++i)); do
#    zsh ./${scpt_path}/sh${i}*.sh
#done
#for ((i = 74; i <= 76; ++i)); do
#    zsh ./${scpt_path}/sh${i}*.sh
#done
