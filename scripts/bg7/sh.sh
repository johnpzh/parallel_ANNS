#!/usr/local/bin/zsh

data_dir="/scratch/zpeng/data"
app_tag="worker_std_deviation"
num_t=32
bin="python ../scripts/test78.PSS_v5_work_std_deviation_ranged_L.py ./PSS_v5_worker_std_deviation_LG"

set -x

##################
data="deep100m"
L_master_lower=90
L_master_upper=150
L_master_step=3

for std_dev_threshold in 0.2 0.4 0.8 1.6 3.2; do
    tag="${app_tag}.T${num_t}.SD${std_dev_threshold}"
    eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master_lower} ${L_master_upper} ${L_master_step} 0 0 0 ${std_dev_threshold}
done




set +x


