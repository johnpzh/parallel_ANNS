#!/home/zpeng/local/bin/zsh
set -x
bin=./profile_para_single_query_search_simple_v3_standard_deviation

##################################
## SIFT1M
##################################
base_dir=/scratch/zpeng/data
data_dir="${base_dir}/sift1m"
data_name=sift

#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=92
L_up=92
L_step=1
X_low=10
X_up=10
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=100
L_step=1
X_low=25
X_up=25
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=100
L_step=1
X_low=55
X_up=55
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}
#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=92
L_up=92
L_step=1
X_low=18
X_up=18
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=100
L_step=1
X_low=48
X_up=48
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=113
L_up=${L_low}
L_step=1
X_low=105
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=92
L_up=${L_low}
L_step=1
X_low=24
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=${L_low}
L_step=1
X_low=89
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=226
L_up=${L_low}
L_step=1
X_low=210
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}
set +x

##################################
## GIST1M
##################################
base_dir=/scratch/zpeng/data
data_dir="${base_dir}/gist1m"
data_name=gist

#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=${L_low}
L_step=1
X_low=11
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=${L_low}
L_step=1
X_low=77
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=276
L_up=${L_low}
L_step=1
X_low=273
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}
#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=98
L_up=${L_low}
L_step=1
X_low=35
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=160
L_up=${L_low}
L_step=1
X_low=162
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=570
L_up=${L_low}
L_step=1
X_low=552
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=${L_low}
L_step=1
X_low=67
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=329
L_up=${L_low}
L_step=1
X_low=305
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=1120
L_up=${L_low}
L_step=1
X_low=1102
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

##################################
## DEEP10M
##################################
base_dir=/scratch/zpeng/data
data_dir="${base_dir}/deep1b"
data_name=deep10M

#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=93
L_up=${L_low}
L_step=1
X_low=5
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=${L_low}
L_step=1
X_low=41
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=114
L_up=${L_low}
L_step=1
X_low=117
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=95
L_up=${L_low}
L_step=1
X_low=21
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=100
L_up=${L_low}
L_step=1
X_low=73
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=224
L_up=${L_low}
L_step=1
X_low=242
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=92
L_up=${L_low}
L_step=1
X_low=25
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.99
P_target=0.990
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=136
L_up=${L_low}
L_step=1
X_low=146
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}

###### P@100 0.999
P_target=0.999
output="output.PSS_stddev_${data_name}_T${num_t}_${P_target}.txt"
L_low=448
L_up=${L_low}
L_step=1
X_low=450
X_up=${X_low}
X_step=1
${bin} ${data_dir}/${data_name}_base.fvecs ${data_dir}/${data_name}_query.fvecs ${data_dir}/${data_name}.nsg ${L_low} 100 ${data_dir}/${data_name}.ivecs ${data_dir}/${data_name}.true-100_NN.v2.binary ${num_t} ${L_up} ${L_step} ${X_up} ${X_low} ${X_step} | tee ${output}
set +x