#!/usr/local/bin/zsh

function get_L_low () {
    if (($1 < 100)); then
        echo $1
    elif (( ($1 / $2) < 100)); then
        echo 100
    else
        echo $(($1 / $2))
    fi
}

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
L_seq=89
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=173
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=425
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=89
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=173
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=425
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2

X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=89
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=173
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=425
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
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
L_seq=129
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=599
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=2288
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=129
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=599
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=2288
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=128
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=599
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=2288
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
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
L_seq=92
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=271
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=994
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=92
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=271
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=994
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=92
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=271
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=994
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
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
L_seq=94
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=338
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=1038
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=94
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=338
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=1038
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=94
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=338
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=1038
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
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
L_seq=120
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=733
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=4417
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 4T
num_t=4
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=120
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=733
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=4417
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
###### P@100 0.90
P_target=0.900
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=120
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.99
P_target=0.990
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=733
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###### P@100 0.999
P_target=0.999
tag="PSS_X_and_L_${data}_T${num_t}_${P_target}"
L_seq=4417
L_low=$(get_L_low $L_seq $num_t)
L_up=$((L_low + 8))
L_step=2
X_low=$((L_seq / num_t))
X_up=$((X_low + 8))
X_step=2
eval ${bin} ${base_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

set +x



