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
#data_dir="/scratch/zpeng"
data=gist1m

#####################################
##### PSS
#####################################
bin="python3 ../scripts/test63.find_L_top_m_scale_m.py"
app="profile_find_L_para_single_query_scale_m_search_pure_profiling"
app_label="Scale_M"

M=128
####################
##### GIST1M 2T
####################
for num_t in 8; do
##
    P_target=0.900
    L_Seq=129
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.910
    L_Seq=140
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.920
    L_Seq=152
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.930
    L_Seq=169
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.940
    L_Seq=189
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.950
    L_Seq=214
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.960
    L_Seq=248
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.970
    L_Seq=300
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.980
    L_Seq=390
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.990
    L_Seq=599
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.991
    L_Seq=635
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.992
    L_Seq=671
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.993
    L_Seq=720
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.994
    L_Seq=798
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.995
    L_Seq=883
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.996
    L_Seq=1008
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.997
    L_Seq=1174
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.998
    L_Seq=1499
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.999
    L_Seq=2225
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    ######
    output_files="output.${data}.${app_label}_T${num_t}_P0.9"
    table_file="output.${data}.${app_label}_T${num_t}_collected.table.txt"
#    selected_file="output.${data}.${app_label}_T${num_t}_collected.selected.txt"
    cat ${output_files}*.table.txt > ${table_file}
#    python3 ../scripts/output_find_runtime_above_presicion.py ${table_file} ${selected_file} 0 2
done
set +x


