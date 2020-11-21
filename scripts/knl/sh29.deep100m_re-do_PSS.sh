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

function get_X_low () {
    if (( ($1 / $2) < 1 )); then
        echo 1
    else
        echo $(( $1 / $2))
    fi
}

set -x

data_dir="/home/zpeng/data"
data=deep100m

################
##### SGS
################
##bin="python3 ../scripts/test48.find_L_simple_search.py"
#bin="python3 ../scripts/test46.find_L_simple_search_large_graph.py"
#
#P_target=0.900
#tag="SGS_find_L_P${P_target}"
#L_low=115
#L_up=500
#P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.990
#tag="SGS_find_L_P${P_target}"
#L_low=700
#L_up=1850
#P_ts="0.99 0.991 0.992 0.993 0.994 0.995 0.996 0.997"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.998
#tag="SGS_find_L_P${P_target}"
#L_low=2350
#L_up=2550
#P_ts="0.998"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.999
#tag="SGS_find_L_P${P_target}"
#L_low=4350
#L_up=4550
#P_ts="0.999"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
###################
##### PSS 1T
###################
##bin="python3 ../scripts/test50.find_L_SV3_T1.py"
#bin="python3 ../scripts/test49.find_L_SV3_T1_large_graph.py"
#
#num_t=1
###
#P_target=0.900
#tag="PSS_T${num_t}_P${P_target}"
#L_low=110
#L_up=500
#P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.990
#tag="PSS_T${num_t}_P${P_target}"
#L_low=700
#L_up=1200
#P_ts="0.99 0.991 0.992 0.993 0.994 0.995"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.996
#tag="PSS_T${num_t}_P${P_target}"
#L_low=1300
#L_up=1650
#P_ts="0.996 0.997"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.998
#tag="PSS_T${num_t}_P${P_target}"
#L_low=2050
#L_up=2150
#P_ts="0.998"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.999
#tag="PSS_T${num_t}_P${P_target}"
#L_low=3300
#L_up=3500
#P_ts="0.999"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#

####################################
#### PSS
####################################
#bin="python3 ../scripts/test43.simple_v3_ranged_L.py"
bin="python3 ../scripts/test47.simple_v3_large_graph_ranged_L.py"
#
####################
##### DEEP10M 2T
####################
#num_t=2
###
#P_target=0.900
#L_Seq=120
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 5 ))
#L_up=$((L_low + 10))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 5 ))
#X_up=$((X_low + 10))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.910
#L_Seq=131
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 5 ))
#L_up=$((L_low + 10))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 5 ))
#X_up=$((X_low + 10))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.920
#L_Seq=147
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 5 ))
#L_up=$((L_low + 10))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 5 ))
#X_up=$((X_low + 10))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.930
#L_Seq=163
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 5 ))
#L_up=$((L_low + 10))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 5 ))
#X_up=$((X_low + 10))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.940
#L_Seq=188
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 5 ))
#L_up=$((L_low + 10))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 5 ))
#X_up=$((X_low + 10))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.950
#L_Seq=217
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 5 ))
#L_up=$((L_low + 10))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 5 ))
#X_up=$((X_low + 10))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.960
#L_Seq=260
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 5 ))
#L_up=$((L_low + 10))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 5 ))
#X_up=$((X_low + 10))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.970
#L_Seq=325
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 10 ))
#L_up=$((L_low + 20))
#L_step=2
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 10 ))
#X_up=$((X_low + 20))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.980
#L_Seq=442
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 10 ))
#L_up=$((L_low + 20))
#L_step=2
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 10 ))
#X_up=$((X_low + 20))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.990
#L_Seq=710
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 10 ))
#L_up=$((L_low + 20))
#L_step=2
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 10 ))
#X_up=$((X_low + 20))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.991
#L_Seq=764
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 15 ))
#L_up=$((L_low + 30))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 15 ))
#X_up=$((X_low + 30))
#X_step=3
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.992
#L_Seq=833
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 15 ))
#L_up=$((L_low + 30))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 15 ))
#X_up=$((X_low + 30))
#X_step=3
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.993
#L_Seq=910
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 15 ))
#L_up=$((L_low + 30))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 15 ))
#X_up=$((X_low + 30))
#X_step=3
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.994
#L_Seq=1011
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 20 ))
#L_up=$((L_low + 40))
#L_step=4
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 20 ))
#X_up=$((X_low + 40))
#X_step=4
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.995
#L_Seq=1142
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 20 ))
#L_up=$((L_low + 40))
#L_step=4
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 20 ))
#X_up=$((X_low + 40))
#X_step=4
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.996
#L_Seq=1335
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 20 ))
#L_up=$((L_low + 40))
#L_step=4
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 20 ))
#X_up=$((X_low + 40))
#X_step=4
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.997
#L_Seq=1605
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 20 ))
#L_up=$((L_low + 40))
#L_step=4
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 20 ))
#X_up=$((X_low + 40))
#X_step=4
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.998
#L_Seq=2100
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 20 ))
#L_up=$((L_low + 40))
#L_step=4
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 20 ))
#X_up=$((X_low + 40))
#X_step=4
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.999
#L_Seq=3339
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 20 ))
#L_up=$((L_low + 40))
#L_step=4
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 20 ))
#X_up=$((X_low + 40))
#X_step=4
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

###################
#### DEEP10M 4T
###################
num_t=4
##
#P_target=0.900
#L_Seq=120
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
#L_up=$((L_low + 12))
#L_step=4
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
#X_up=$((X_low + 12))
#X_step=4
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.910
L_Seq=131
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.920
L_Seq=147
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.930
L_Seq=163
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.940
L_Seq=188
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.950
L_Seq=217
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.960
L_Seq=260
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.970
L_Seq=325
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 12 ))
L_up=$((L_low + 24))
L_step=8
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 12 ))
X_up=$((X_low + 24))
X_step=8
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.980
L_Seq=442
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 12 ))
L_up=$((L_low + 24))
L_step=8
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 12 ))
X_up=$((X_low + 24))
X_step=8
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.990
L_Seq=710
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 12 ))
L_up=$((L_low + 24))
L_step=8
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 12 ))
X_up=$((X_low + 24))
X_step=8
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.991
L_Seq=764
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 18 ))
L_up=$((L_low + 36))
L_step=12
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 18 ))
X_up=$((X_low + 36))
X_step=12
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.992
L_Seq=833
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 18 ))
L_up=$((L_low + 36))
L_step=12
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 18 ))
X_up=$((X_low + 36))
X_step=12
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.993
L_Seq=910
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 18 ))
L_up=$((L_low + 36))
L_step=12
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 18 ))
X_up=$((X_low + 36))
X_step=12
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.994
L_Seq=1011
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.995
L_Seq=1142
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.996
L_Seq=1335
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.997
L_Seq=1605
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.998
L_Seq=2100
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.999
L_Seq=3339
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

####################
##### DEEP10M 8T
####################
num_t=8
##
P_target=0.900
L_Seq=120
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.910
L_Seq=131
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.920
L_Seq=147
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.930
L_Seq=163
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.940
L_Seq=188
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.950
L_Seq=217
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.960
L_Seq=260
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
L_up=$((L_low + 12))
L_step=4
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
X_up=$((X_low + 12))
X_step=4
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.970
L_Seq=325
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 12 ))
L_up=$((L_low + 24))
L_step=8
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 12 ))
X_up=$((X_low + 24))
X_step=8
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.980
L_Seq=442
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 12 ))
L_up=$((L_low + 24))
L_step=8
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 12 ))
X_up=$((X_low + 24))
X_step=8
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.990
L_Seq=710
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 12 ))
L_up=$((L_low + 24))
L_step=8
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 12 ))
X_up=$((X_low + 24))
X_step=8
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.991
L_Seq=764
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 18 ))
L_up=$((L_low + 36))
L_step=12
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 18 ))
X_up=$((X_low + 36))
X_step=12
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.992
L_Seq=833
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 18 ))
L_up=$((L_low + 36))
L_step=12
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 18 ))
X_up=$((X_low + 36))
X_step=12
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.993
L_Seq=910
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 18 ))
L_up=$((L_low + 36))
L_step=12
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 18 ))
X_up=$((X_low + 36))
X_step=12
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.994
L_Seq=1011
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.995
L_Seq=1142
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.996
L_Seq=1335
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.997
L_Seq=1605
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.998
L_Seq=2100
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

P_target=0.999
L_Seq=3339
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 24 ))
L_up=$((L_low + 48))
L_step=16
X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 24 ))
X_up=$((X_low + 48))
X_step=16
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

set +x


