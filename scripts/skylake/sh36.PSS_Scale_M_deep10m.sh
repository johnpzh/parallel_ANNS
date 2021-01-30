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

#data_dir="/home/zpeng/data"
data_dir="/scratch/zpeng"
data=deep10m

################
##### SGS
################
#bin="python3 ../scripts/test48.find_L_simple_search.py"
#
#P_target=0.900
#tag="SGS_find_L_P${P_target}"
#L_low=90
#L_up=100
#P_ts="0.9 0.91 0.92 0.93 0.94"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.950
#tag="SGS_find_L_P${P_target}"
#L_low=100
#L_up=200
#P_ts="0.95 0.96 0.97 0.98"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.990
#tag="SGS_find_L_P${P_target}"
#L_low=200
#L_up=300
#P_ts="0.99 0.991"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.992
#tag="SGS_find_L_P${P_target}"
#L_low=300
#L_up=400
#P_ts="0.992 0.993 0.994 0.995"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.996
#tag="SGS_find_L_P${P_target}"
#L_low=400
#L_up=500
#P_ts="0.996"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.997
#tag="SGS_find_L_P${P_target}"
#L_low=500
#L_up=600
#P_ts="0.997"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.998
#tag="SGS_find_L_P${P_target}"
#L_low=600
#L_up=700
#P_ts="0.998"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.999
#tag="SGS_find_L_P${P_target}"
#L_low=950
#L_up=1000
#P_ts="0.999"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#######
#output_files="output.${data}.SGS_find_L_P0.9"
#table_file="output.${data}.SGS_find_L_collected.table.txt"
#cat ${output_files}*.table.txt > ${table_file}

#
###################
##### PSS 1T
###################
#bin="python3 ../scripts/test50.find_L_SV3_T1.py"
#
#num_t=1
###
#P_target=0.900
#tag="PSS_T${num_t}_P${P_target}"
#L_low=90
#L_up=100
#P_ts="0.9 0.91 0.92 0.93 0.94"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.950
#tag="PSS_T${num_t}_P${P_target}"
#L_low=100
#L_up=200
#P_ts="0.95 0.96 0.97 0.98"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.990
#tag="PSS_T${num_t}_P${P_target}"
#L_low=200
#L_up=300
#P_ts="0.99 0.991"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.992
#tag="PSS_T${num_t}_P${P_target}"
#L_low=300
#L_up=400
#P_ts="0.992 0.993 0.994 0.995"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.996
#tag="PSS_T${num_t}_P${P_target}"
#L_low=400
#L_up=500
#P_ts="0.996"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.997
#tag="PSS_T${num_t}_P${P_target}"
#L_low=500
#L_up=550
#P_ts="0.997"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.998
#tag="PSS_T${num_t}_P${P_target}"
#L_low=600
#L_up=650
#P_ts="0.998"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}
#
#P_target=0.999
#tag="PSS_T${num_t}_P${P_target}"
#L_low=850
#L_up=900
#P_ts="0.999"
#eval ${bin} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}


####################################
#### PSS
####################################
bin="python3 ../scripts/test43.simple_v3_ranged_L.py"
app="profile_para_single_query_search_simple_v3_ranged_scale_M"
#bin="python3 ../scripts/test47.simple_v3_large_graph_ranged_L.py"

###################
#### DEEP10M 2T
###################
#for num_t in 1 2 4 8; do
for num_t in 32; do
#
    P_target=0.900
    L_Seq=92
    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
    L_up=$((L_low + 2))
    L_step=1
    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
    X_up=$((X_low + 2))
    X_step=1
    tag="PSS_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.910
#    L_Seq=94
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.920
#    L_Seq=95
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.930
#    L_Seq=97
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.940
#    L_Seq=98
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.950
#    L_Seq=102
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.960
#    L_Seq=118
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.970
#    L_Seq=142
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.980
#    L_Seq=183
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 2))
#    L_step=1
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 2))
#    X_step=1
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

    P_target=0.990
    L_Seq=270
    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
    L_up=$((L_low + 4))
    L_step=2
    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
    X_up=$((X_low + 4))
    X_step=2
    tag="PSS_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.991
#    L_Seq=284
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.992
#    L_Seq=304
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.993
#    L_Seq=327
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.994
#    L_Seq=355
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 4))
#    L_step=2
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 4))
#    X_step=2
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

    P_target=0.995
    L_Seq=392
    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
    L_up=$((L_low + 4))
    L_step=2
    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
    X_up=$((X_low + 4))
    X_step=2
    tag="PSS_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.996
#    L_Seq=441
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 8))
#    L_step=4
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 8))
#    X_step=4
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

    P_target=0.997
    L_Seq=509
    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
    L_up=$((L_low + 8))
    L_step=4
    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
    X_up=$((X_low + 8))
    X_step=4
    tag="PSS_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
#    P_target=0.998
#    L_Seq=630
#    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
#    L_up=$((L_low + 8))
#    L_step=4
#    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
#    X_up=$((X_low + 8))
#    X_step=4
#    tag="PSS_T${num_t}_P${P_target}"
#    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

    P_target=0.999
    L_Seq=890
    L_low=$(( $(get_L_low ${L_Seq} ${num_t}) ))
    L_up=$((L_low + 16))
    L_step=8
    X_low=$(( $(get_X_low ${L_Seq} ${num_t}) ))
    X_up=$((X_low + 16))
    X_step=8
    tag="PSS_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

    ######
    output_files="output.${data}.PSS_T${num_t}_P0.9"
    table_file="output.${data}.PSS_T${num_t}_collected.table.txt"
    selected_file="output.${data}.PSS_T${num_t}_collected.selected.txt"
    cat ${output_files}*.table.txt > ${table_file}
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
#    0.900 0.910 0.920 0.930 0.940 \
#    0.950 0.960 0.970 0.980 0.990 \
#    0.991 0.992 0.993 0.994 0.995 \
#    0.996 0.997 0.998 0.999
    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2 \
    0.900 0.990 0.995 0.997 0.999

done

set +x


