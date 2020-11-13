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
    if (( ($1 / %2) < 1 )); then
        echo 1
    else
        echo $(( $1 / $2))
    fi
}

set -x

data_dir="/scratch/zpeng/sift1m"
data=sift1m

###############
#### SGS
###############
bin="python3 ../scripts/test48.find_L_simple_search.py"

P_target=0.900
tag="SGS_find_L_P${P_target}"
L_low=90
L_up=100
P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.970
tag="SGS_find_L_P${P_target}"
L_low=100
L_up=200
P_ts="0.97 0.98 0.99 0.991 0.992"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.993
tag="SGS_find_L_P${P_target}"
L_low=200
L_up=300
P_ts="0.993 0.994 0.995 0.996 0.997"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.998
tag="SGS_find_L_P${P_target}"
L_low=300
L_up=350
P_ts="0.998"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.999
tag="SGS_find_L_P${P_target}"
L_low=400
L_up=450
P_ts="0.999"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

##################
#### PSS 1T
##################
bin="python3 ../scripts/test50.find_L_SV3_T1.py"

num_t=1
##
P_target=0.900
tag="PSS_T${num_t}_P${P_target}"
L_low=90
L_up=100
P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.970
tag="PSS_T${num_t}_P${P_target}"
L_low=100
L_up=200
P_ts="0.97 0.98 0.99 0.991 0.992 0.993"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.994
tag="PSS_T${num_t}_P${P_target}"
L_low=200
L_up=300
P_ts="0.994 0.995 0.996 0.997"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.998
tag="PSS_T${num_t}_P${P_target}"
L_low=300
L_up=350
P_ts="0.998"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.999
tag="PSS_T${num_t}_P${P_target}"
L_low=400
L_up=450
P_ts="0.999"
eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}


#####################################
##### PSS
#####################################
#bin="python3 ../scripts/test43.simple_v3_ranged_L.py"
##bin="python3 ../scripts/test47.simple_v3_large_graph_ranged_L.py"
#
####################
##### SIFT1M 2T
####################
#num_t=2
###
#P_target=0.900
#L_Seq=94
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.910
#L_Seq=96
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.920
#L_Seq=99
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.930
#L_Seq=108
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.940
#L_Seq=120
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.950
#L_Seq=135
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.960
#L_Seq=156
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.970
#L_Seq=186
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.980
#L_Seq=235
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
#L_up=$((L_low + 12))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.990
#L_Seq=339
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.991
#L_Seq=360
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.992
#L_Seq=381
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.993
#L_Seq=408
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.994
#L_Seq=442
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.995
#L_Seq=480
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.996
#L_Seq=534
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.997
#L_Seq=618
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
#X_up=$((X_low + 12))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.998
#L_Seq=752
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
#X_up=$((X_low + 12))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.999
#L_Seq=1040
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
#X_up=$((X_low + 12))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
####################
##### SIFT1M 4T
####################
#num_t=4
###
#P_target=0.900
#L_Seq=94
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.910
#L_Seq=96
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.920
#L_Seq=99
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.930
#L_Seq=108
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.940
#L_Seq=120
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.950
#L_Seq=135
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.960
#L_Seq=156
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.970
#L_Seq=186
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 1 ))
#L_up=$((L_low + 2))
#L_step=1
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 1 ))
#X_up=$((X_low + 2))
#X_step=1
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.980
#L_Seq=235
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 6 ))
#L_up=$((L_low + 12))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.990
#L_Seq=339
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.991
#L_Seq=360
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.992
#L_Seq=381
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.993
#L_Seq=408
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.994
#L_Seq=442
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.995
#L_Seq=480
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.996
#L_Seq=534
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 4 ))
#X_up=$((X_low + 8))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.997
#L_Seq=618
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
#X_up=$((X_low + 12))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.998
#L_Seq=752
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
#X_up=$((X_low + 12))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#P_target=0.999
#L_Seq=1040
#L_low=$(( $(get_L_low ${L_Seq} ${num_t}) - 9 ))
#L_up=$((L_low + 18))
#L_step=3
#X_low=$(( $(get_X_low ${L_Seq} ${num_t}) - 6 ))
#X_up=$((X_low + 12))
#X_step=2
#tag="PSS_T${num_t}_P${P_target}"
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
####################
##### SIFT1M 8T
####################
#num_t=8
#L_low=
#L_up=
#L_step=
#X_low=
#X_up=
#X_step=
#eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}


set +x


