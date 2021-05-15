
base_dir="/scratch/zpeng/data"

data_dir="${base_dir}/sift1m"
data="sift"
echo -n "${data} ... "
./PSS_v5_LG_in_degree ${data_dir}/${data}_base.fvecs ${data_dir}/${data}.nsg ${data_dir}/${data}_reorder_map.in_degree.binary
echo "done."

data_dir="${base_dir}/sift1b"
data="sift100M"
echo -n "${data} ... "
./PSS_v5_LG_in_degree ${data_dir}/${data}_base.fvecs ${data_dir}/${data}.nsg ${data_dir}/${data}_reorder_map.in_degree.binary
echo "done."

data_dir="${base_dir}/deep1b"
data="deep100M"
echo -n "${data} ... "
./PSS_v5_LG_in_degree ${data_dir}/${data}_base.fvecs ${data_dir}/${data}.nsg ${data_dir}/${data}_reorder_map.in_degree.binary
echo "done."