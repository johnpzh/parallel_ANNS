data_dir="/scratch/zpeng"
data="deep1m"

bin="python3 ../scripts/test45.hnsw_find_ef.py"
app="hnsw_search"
app_tag="hnsw"

P_target=0.900
tag="${app_tag}_find_ef_P${P_target}"
ef_low=1
ef_up=2
P_ts="0.900 0.910"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

P_target=0.920
tag="${app_tag}_find_ef_P${P_target}"
ef_low=100
ef_up=300
P_ts="0.920 0.930 0.940 \
0.950 0.960 0.970 0.980"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

P_target=0.990
tag="${app_tag}_find_ef_P${P_target}"
ef_low=300
ef_up=500
P_ts="0.990 \
0.991 0.992 0.993 0.994"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

P_target=0.995
tag="${app_tag}_find_ef_P${P_target}"
ef_low=500
ef_up=600
P_ts="0.995 \
0.996"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

P_target=0.997
tag="${app_tag}_find_ef_P${P_target}"
ef_low=600
ef_up=800
P_ts="0.997 0.998"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

P_target=0.999
tag="${app_tag}_find_ef_P${P_target}"
ef_low=1050
ef_up=1080
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

####
table_file="output.${data}.${app_tag}_find_ef_collected.table.txt"
cat output.${data}.${app_tag}_find_ef_P*.table.txt > ${table_file}