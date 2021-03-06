#!/usr/local/bin/zsh

## get_L_master(L_seq)
function get_L_master () {
    if (( $1 < 100 )); then
#        echo $1
        echo $(( $1 + 1 ))
    else
        echo 102
    fi
}

## get_L_low(L_seq, num_t)
function get_L_low () {
    base=$(($1 / $2))
    offset=$(( $1 / 2 ))
    if (( offset < 1 )); then
        offset=1
    fi
    low=$(( base - offset ))
    if (( low < 1 )); then
        low=1
    fi

    echo $low
}

## get_L_up(L_seq, num_t)
function get_L_up () {
    base=$(($1 / $2))
    offset=$(( $1 / 2 ))
    if (( offset == 0 )); then
        offset=1
    fi
    up=$(( base + offset ))
    if (( up < 100 )); then
        up=100
    fi

    echo $up
}

## get_L_up(L_seq, num_t)
function get_X () {
    X=$(( $1 / $2 + 4))
    echo $X
}

set -x

#data_dir="/home/zpeng/data"
data_dir="/scratch/zpeng"
data=sift1m

###################
##### PSS 1T
###################
#bin="python3 ../scripts/test49.find_L_PSS_v5_profile.py"
#app="profile_find_L_para_single_query_search_PSS_v5_profile"
#
#num_t=1
###
#P_target=0.900
#tag="PSS_T${num_t}_P${P_target}"
#L_low=90
#L_up=100
#P_ts="0.9"
##P_ts="0.9 0.91 0.92 0.93 0.94 0.95 0.96"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.970
#tag="PSS_T${num_t}_P${P_target}"
#L_low=100
#L_up=200
#P_ts="0.99"
##P_ts="0.97 0.98 0.99 0.991 0.992 0.993"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.994
#tag="PSS_T${num_t}_P${P_target}"
#L_low=200
#L_up=300
#P_ts="0.995 0.997"
##P_ts="0.994 0.995 0.996 0.997"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
##P_target=0.998
##tag="PSS_T${num_t}_P${P_target}"
##L_low=300
##L_up=350
##P_ts="0.998"
##eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.999
#tag="PSS_T${num_t}_P${P_target}"
#L_low=400
#L_up=450
#P_ts="0.999"
#eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#output_files="output.${data}.PSS_T${num_t}_P0.9"
#table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
#cat ${output_files}*.table.txt > ${table_file}


####################################
#### PSS Multiple Threads
####################################
bin="python3 ../scripts/test49.find_L_PSS_v5_profile.py"
app="profile_find_L_para_single_query_search_PSS_v5_profile"

####################
##### SIFT1M 2T
####################
P_target=0.999
L_Seq=428

num_t=8
L_master=106
X=57
#L_master=$(( $(get_L_master ${L_Seq}) ))
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
#X=$(( $(get_X ${L_Seq} ${num_t}) ))
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
output_files="output.${data}.PSS_T${num_t}_P0.9"
table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

num_t=16
L_master=106
X=30
#L_master=$(( $(get_L_master ${L_Seq}) ))
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
#X=$(( $(get_X ${L_Seq} ${num_t}) ))
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
output_files="output.${data}.PSS_T${num_t}_P0.9"
table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

num_t=32
L_master=108
X=17
#L_master=$(( $(get_L_master ${L_Seq}) ))
L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
#X=$(( $(get_X ${L_Seq} ${num_t}) ))
tag="PSS_T${num_t}_P${P_target}"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
output_files="output.${data}.PSS_T${num_t}_P0.9"
table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}


##for num_t in 64; do
#for num_t in 1 2 4 8 16 20; do
###
##    P_target=0.900
##    L_Seq=90
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.910
##    L_Seq=91
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.920
##    L_Seq=92
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.930
##    L_Seq=93
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.940
##    L_Seq=95
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.950
##    L_Seq=96
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.960
##    L_Seq=98
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.970
##    L_Seq=105
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.980
##    L_Seq=128
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.990
##    L_Seq=173
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.991
##    L_Seq=181
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.992
##    L_Seq=190
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.993
##    L_Seq=201
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.994
##    L_Seq=214
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.995
##    L_Seq=231
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.996
##    L_Seq=253
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.997
##    L_Seq=282
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
##
##    P_target=0.998
##    L_Seq=330
##    L_master=$(( $(get_L_master ${L_Seq}) ))
##    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
##    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
##    X=$(( $(get_X ${L_Seq} ${num_t}) ))
##    tag="PSS_T${num_t}_P${P_target}"
##    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
#
#    P_target=0.999
#    L_Seq=428
#    L_master=$(( $(get_L_master ${L_Seq}) ))
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((  $(get_L_up ${L_Seq} ${num_t})  ))
#    X=$(( $(get_X ${L_Seq} ${num_t}) ))
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_low} ${L_up} ${X} ${P_target}
#
#    output_files="output.${data}.PSS_T${num_t}_P0.9"
#    table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
#    cat ${output_files}*.table.txt > ${table_file}
#
##    ######
##    output_files="output.${data}.PSS_T${num_t}_P0.9"
##    table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
##    selected_file="output.${data}.PSS_T${num_t}_collected.selected.txt"
##    cat ${output_files}*.table.txt > ${table_file}
###    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
###    0.900 0.910 0.920 0.930 0.940 \
###    0.950 0.960 0.970 0.980 0.990 \
###    0.991 0.992 0.993 0.994 0.995 \
###    0.996 0.997 0.998 0.999
##    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
##    0.900 0.990 0.995 0.997 0.999
#
#
#done



set +x


