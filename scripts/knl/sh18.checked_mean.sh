#!/usr/local/bin/zsh

set -x
bin="python3 ../scripts/test59.checked_mean_simple_v3_ranged_L.py"
#### SIFT1M 8T
###### 0.90
data=sift1m
num_t=8
PT=0.900
L_low=91
L_up=${L_low}
X_low=5
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=100
L_up=100
X_low=24
X_up=25
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=100
L_up=100
X_low=53
X_up=54
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### SIFT1M 4T
###### 0.90
num_t=4
PT=0.900
L_low=92
L_up=92
X_low=19
X_up=20
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=100
L_up=${L_low}
X_low=45
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=113
L_up=${L_low}
X_low=104
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### SIFT1M 2T
###### 0.90
num_t=2
PT=0.900
L_low=92
L_up=${L_low}
X_low=24
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=100
L_up=${L_low}
X_low=88
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=220
L_up=${L_low}
X_low=241
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### GIST1M 8T
###### 0.90
data=gist1m
num_t=8
PT=0.900
L_low=100
L_up=${L_low}
X_low=10
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=100
L_up=${L_low}
X_low=76
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=276
L_up=${L_low}
X_low=272
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### GIST1M 4T
###### 0.90
num_t=4
PT=0.900
L_low=98
L_up=${L_low}
X_low=34
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=160
L_up=${L_low}
X_low=161
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=570
L_up=${L_low}
X_low=551
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### GIST1M 2T
###### 0.90
num_t=2
PT=0.900
L_low=100
L_up=${L_low}
X_low=66
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=329
L_up=${L_low}
X_low=304
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=1120
L_up=${L_low}
X_low=1102
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### DEEP10M 8T
###### 0.90
data=deep10m
num_t=8
PT=0.900
L_low=93
L_up=${L_low}
X_low=4
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=100
L_up=${L_low}
X_low=40
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=114
L_up=${L_low}
X_low=116
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### DEEP10M 4T
###### 0.90
num_t=4
PT=0.900
L_low=95
L_up=${L_low}
X_low=20
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=100
L_up=${L_low}
X_low=72
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=224
L_up=${L_low}
X_low=241
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

#### DEEP10M 2T
###### 0.90
num_t=2
PT=0.900
L_low=92
L_up=${L_low}
X_low=24
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.99
PT=0.990
L_low=136
L_up=${L_low}
X_low=145
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1
###### 0.999
PT=0.999
L_low=448
L_up=${L_low}
X_low=449
X_up=$((X_low + 1))
eval ${bin} ~/data ${data} PSS_checked_mean_T${num_t}_${PT} ${num_t} ${L_low} ${L_up} 1 ${X_low} ${X_up} 1

set +x


