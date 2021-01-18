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

data_dir="/scratch/zpeng"
data=sift100m



####################################
#### PSS
####################################
bin="python3 ../scripts/test63.find_L_top_m_scale_m.py"
app="profile_find_L_para_single_query_top_m_search_pure_large_graph"
app_label="Top_M"
M=64

###################
#### DEEP10M 2T
###################
for num_t in 8; do
##
    P_target=0.900
    L_Seq=95
    L_low=$(( 90 ))
    L_up=$(( L_Seq + 5 ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.910
    L_Seq=97
    L_low=$(( 90 ))
    L_up=$(( L_Seq + 5 ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.920
    L_Seq=99
    L_low=$(( 90 ))
    L_up=$(( L_Seq + 5 ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.930
    L_Seq=108
    offset=10
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.940
    L_Seq=120
    offset=16
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.950
    L_Seq=135
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.960
    L_Seq=156
    offset=32
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.970
    L_Seq=186
    offset=64
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.980
    L_Seq=235
    offset=128
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.990
    L_Seq=339
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.991
    L_Seq=359
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.992
    L_Seq=381
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.993
    L_Seq=407
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.994
    L_Seq=439
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.995
    L_Seq=478
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.996
    L_Seq=529
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.997
    L_Seq=611
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.998
    L_Seq=741
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.999
    L_Seq=999
    offset=256
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


