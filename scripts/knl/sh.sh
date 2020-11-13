#!/usr/local/bin/zsh

set -x

base_dir="/home/zpeng/data"

#### Small Graphs
bin="python3 ../scripts/test43.simple_v3_ranged_L.py"
##################################
## SIFT1M
##################################
data="sift1m"

#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=91
L_up=${L_low}
L_step=1
L_seq=89
X_low=$((L_seq / num_t))
X_up=$((X_low + 2))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=173
X_low=$((L_seq / num_t))
X_up=$((X_low + 2))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=425
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=92
L_up=${L_low}
L_step=1
L_seq=89
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=173
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=113
L_up=${L_low}
L_step=1
L_seq=425
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=92
L_up=${L_low}
L_step=1
L_seq=89
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=173
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=220
L_up=${L_low}
L_step=1
L_seq=425
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#
###################################
### GIST1M
###################################
data="gist1m"
#
#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=129
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=599
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=276
L_up=${L_low}
L_step=1
L_seq=2288
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=98
L_up=${L_low}
L_step=1
L_seq=129
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=160
L_up=${L_low}
L_step=1
L_seq=599
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=570
L_up=${L_low}
L_step=1
L_seq=2288
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=128
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=329
L_up=${L_low}
L_step=1
L_seq=599
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=1120
L_up=${L_low}
L_step=1
L_seq=2288
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

##################################
## DEEP10M
##################################
data="deep10m"

#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=93
L_up=${L_low}
L_step=1
L_seq=92
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=271
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=114
L_up=${L_low}
L_step=1
L_seq=994
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=95
L_up=${L_low}
L_step=1
L_seq=92
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=271
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=224
L_up=${L_low}
L_step=1
L_seq=994
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=92
L_up=${L_low}
L_step=1
L_seq=92
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=136
L_up=${L_low}
L_step=1
L_seq=271
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=448
L_up=${L_low}
L_step=1
L_seq=994
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### Large Graphs
bin="python3 ../scripts/test47.simple_v3_large_graph_ranged_L.py"
##################################
## SIFT100M
##################################
data="sift100m"

#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=91
L_up=${L_low}
L_step=1
L_seq=94
X_low=$((L_seq / num_t))
X_up=$((X_low + 2))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=338
X_low=$((L_seq / num_t))
X_up=$((X_low + 2))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=1038
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=92
L_up=${L_low}
L_step=1
L_seq=94
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=338
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=113
L_up=${L_low}
L_step=1
L_seq=1038
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=92
L_up=${L_low}
L_step=1
L_seq=94
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=338
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=220
L_up=${L_low}
L_step=1
L_seq=1038
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

##################################
## DEEP100M
##################################
data="deep100m"

#### 8T
num_t=8
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=91
L_up=${L_low}
L_step=1
L_seq=120
X_low=$((L_seq / num_t))
X_up=$((X_low + 2))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=733
X_low=$((L_seq / num_t))
X_up=$((X_low + 2))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=4417
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=92
L_up=${L_low}
L_step=1
L_seq=120
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=733
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=113
L_up=${L_low}
L_step=1
L_seq=4417
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=92
L_up=${L_low}
L_step=1
L_seq=120
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=100
L_up=${L_low}
L_step=1
L_seq=733
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_low=220
L_up=${L_low}
L_step=1
L_seq=4417
X_low=$((L_seq / num_t))
X_up=$((X_low + 1))
X_step=1
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

set +x



