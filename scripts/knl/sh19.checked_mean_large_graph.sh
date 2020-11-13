#!/usr/local/bin/zsh

set -x
bin="python3 ../scripts/test60.checked_mean_simple_v3_large_graph_ranged_L.py"
##### DEEP100M 8T
####### 0.90
data=deep100m
#num_t=8
#PT=0.900
#L_low=96
#L_up=${L_low}
#X_low=14
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.99
#PT=0.990
#L_low=100
#L_up=${L_low}
#X_low=94
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.999
#PT=0.999
#L_low=448
#L_up=${L_low}
#X_low=417
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
#
##### DEEP100M 4T
####### 0.90
#num_t=4
#PT=0.900
#L_low=100
#L_up=${L_low}
#X_low=28
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.99
#PT=0.990
#L_low=220
#L_up=${L_low}
#X_low=193
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.999
#PT=0.999
#L_low=896
#L_up=${L_low}
#X_low=833
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### DEEP100M 2T
###### 0.90
num_t=2
#PT=0.900
#L_low=100
#L_up=${L_low}
#X_low=64
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=388
L_up=${L_low}
X_low=369
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=1984
L_up=2174
X_low=1024
X_up=2176
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 64 ${X_low} ${X_up} 64

##### SIFT100M 8T
####### 0.90
#data=sift100m
#num_t=8
#PT=0.900
#L_low=92
#L_up=${L_low}
#X_low=16
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.99
#PT=0.990
#L_low=100
#L_up=${L_low}
#X_low=50
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.999
#PT=0.999
#L_low=188
#L_up=${L_low}
#X_low=129
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
#
##### SIFT100M 4T
####### 0.90
#num_t=4
#PT=0.900
#L_low=94
#L_up=${L_low}
#X_low=26
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.99
#PT=0.990
#L_low=100
#L_up=${L_low}
#X_low=92
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.999
#PT=0.999
#L_low=276
#L_up=${L_low}
#X_low=257
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
#
##### SIFT100M 2T
####### 0.90
#num_t=2
#PT=0.900
#L_low=96
#L_up=${L_low}
#X_low=48
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.99
#PT=0.990
#L_low=212
#L_up=${L_low}
#X_low=177
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
####### 0.999
#PT=0.999
#L_low=592
#L_up=${L_low}
#X_low=513
#X_up=$((X_low + 1))
#eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

set +x


