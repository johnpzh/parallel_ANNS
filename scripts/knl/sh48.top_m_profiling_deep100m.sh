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

####################################
#### PSS
####################################
bin="python3 ../scripts/test63.find_L_top_m_scale_m.py"
app="profile_find_L_para_single_query_top_m_search_pure_large_graph_profiling"
app_label="Top_M"
M=64

###################
#### DEEP10M 2T
###################
for num_t in 32; do
##
    P_target=0.900
    L_Seq=120
    offset=32
    L_low=$(( 90 ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.910
    L_Seq=131
    offset=32
    L_low=$(( 90 ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.920
    L_Seq=147
    offset=32
    L_low=$(( 90 ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.930
    L_Seq=163
    offset=32
    L_low=$(( 90 ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.940
    L_Seq=188
    offset=32
    L_low=$(( 90 ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.950
    L_Seq=217
    offset=32
    L_low=$(( 90 ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.960
    L_Seq=260
    offset=32
    L_low=$(( 90 ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.970
    L_Seq=325
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.980
    L_Seq=442
    offset=256
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.990
    L_Seq=710
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.991
    L_Seq=764
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.992
    L_Seq=833
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.993
    L_Seq=910
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.994
    L_Seq=1011
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.995
    L_Seq=1142
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.996
    L_Seq=1335
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.997
    L_Seq=1605
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.998
    L_Seq=2100
    offset=512
    L_low=$(( L_Seq - offset ))
    L_up=$(( L_Seq ))
    tag="${app_label}_T${num_t}_P${P_target}"
    eval ${bin} ${app} ${data_dir} ${data} ${tag} ${num_t} ${L_low} ${L_up} ${M} ${P_target}

    P_target=0.999
    L_Seq=3339
    offset=512
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


