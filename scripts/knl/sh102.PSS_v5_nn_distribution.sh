#data_dir="/scratch/zpeng/deep1b"

base_dir="/home/zpeng/data"
q_id=17

#################################
#### SIFT1M
#################################
data_dir="${base_dir}/sift1m"
data="sift"

num_t=1
L=413
X=9999999
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=2
L=214
X=214
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=4
L=107
X=107
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=8
L=100
X=57
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=16
L=100
X=30
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=32
L=100
X=17
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=64
L=104
X=10
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

#################################
#### GIST1M
#################################
data_dir="${base_dir}/gist1m"
data="gist"

num_t=1
L=2224
X=999999
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=2
L=1112
X=1128
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=4
L=564
X=572
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=8
L=278
X=294
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=16
L=147
X=147
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=32
L=108
X=77
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=64
L=108
X=42
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

#################################
#### DEEP10M
#################################
data_dir="${base_dir}/deep1b"
data="deep10M"

num_t=1
L=888
X=9999999
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=2
L=445
X=461
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=4
L=230
X=230
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=8
L=119
X=119
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=16
L=100
X=63
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=32
L=116
X=35
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=64
L=108
X=21
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

#################################
#### SIFT100M
#################################
data_dir="${base_dir}/sift1b"
data="sift100M"

num_t=1
L=999
X=9999999
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=2
L=507
X=515
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=4
L=257
X=265
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=8
L=140
X=140
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=16
L=108
X=78
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=32
L=100
X=39
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

num_t=64
L=100
X=31
output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}

##################################
##### DEEP100M
##################################
#data_dir="${base_dir}/deep1b"
#data="deep100M"
#
#num_t=1
#L=3340
#X=9999999
#output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
#./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}
#
#num_t=2
#L=1669
#X=1685
#output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
#./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}
#
#num_t=4
#L=834
#X=842
#output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
#./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}
#
#num_t=8
#L=417
#X=433
#output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
#./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}
#
#num_t=16
#L=208
#X=208
#output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
#./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}
#
#num_t=32
#L=104
#X=112
#output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
#./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}
#
#num_t=64
#L=100
#X=68
#output="output.PSS_v5_nn_distri_${data}_Q${q_id}_T${num_t}.txt"
#./PSS_v5_LG_nn_distribution ${data_dir}/${data}_base.fvecs ${data_dir}/${data}_query.fvecs ${data_dir}/${data}.nsg 100 output.ivecs ${data_dir}/${data}.true-100_NN.v2.binary ${num_t} ${L} ${L} 1 0 0 0 ${X} ${X} 1 0 0 0 ${q_id} | tee ${output}