#!/usr/local/bin/zsh

set -x
#bin="../scripts/test47.simple_v3_large_graph_ranged_L.py"
bin="../scripts/test43.simple_v3_ranged_L.py"
data=gist1m
#### 8T
num_t=8
L_low=100
L_up=100
L_step=2
X_low=10
X_up=100
X_step=3
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=100
L_up=104
L_step=2
X_low=100
X_up=110
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=128
L_up=130
L_step=2
X_low=110
X_up=120
X_step=3
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=146
L_up=148
L_step=2
X_low=128
X_up=130
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=150
L_up=152
L_step=2
X_low=150
X_up=154
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=214
L_up=216
L_step=2
X_low=184
X_up=186
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=276
L_up=279
L_step=2
X_low=272
X_up=273
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
#
##### 4T
num_t=4
L_low=98
L_up=100
L_step=2
X_low=34
X_up=36
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=120
L_up=130
L_step=2
X_low=40
X_up=130
X_step=4
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=140
L_up=142
L_step=2
X_low=100
X_up=140
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=160
L_up=162
L_step=2
X_low=150
X_up=160
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=180
L_up=182
L_step=2
X_low=170
X_up=180
X_step=3
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=200
L_up=202
L_step=2
X_low=190
X_up=200
X_step=3
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=240
L_up=242
L_step=2
X_low=200
X_up=240
X_step=4
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=260
L_up=262
L_step=2
X_low=240
X_up=260
X_step=4
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=300
L_up=302
L_step=2
X_low=280
X_up=300
X_step=4
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=380
L_up=390
L_step=2
X_low=360
X_up=390
X_step=3
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=560
L_up=570
L_step=2
X_low=550
X_up=560
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

#### 2T
num_t=2
L_low=98
L_up=100
L_step=2
X_low=60
X_up=80
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=106
L_up=110
L_step=2
X_low=90
X_up=110
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=150
L_up=160
L_step=2
X_low=130
X_up=150
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=196
L_up=200
L_step=2
X_low=190
X_up=200
X_step=3
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=328
L_up=330
L_step=2
X_low=300
X_up=330
X_step=3
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=362
L_up=364
L_step=2
X_low=340
X_up=340
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=368
L_up=390
L_step=2
X_low=360
X_up=372
X_step=3
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=454
L_up=456
L_step=2
X_low=450
X_up=456
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=514
L_up=516
L_step=2
X_low=510
X_up=512
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=600
L_up=602
L_step=2
X_low=590
X_up=602
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=750
L_up=752
L_step=2
X_low=750
X_up=752
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}
L_low=1120
L_up=1122
L_step=2
X_low=1102
X_up=1110
X_step=2
${bin} ~/data ${data} PSS_T${num_t}_L${L_low}-${L_up} ${num_t} ${L_low} ${L_up} ${L_step} ${X_low} ${X_up} ${X_step}

##### SIFT100M 1T
## for backup
## from sh2.PSS_sift100M_T1_find_L
#python3 ../scripts/knl/test49.0.90-0.98.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L90-250 90 250 0.0
#python3 ../scripts/knl/test49.0.990-0.995.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L250-500 250 500 0.0
#python3 ../scripts/knl/test49.0.996.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L400-550 400 550 0.0
#python3 ../scripts/knl/test49.0.997.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L500-650 500 650 0.0
#python3 ../scripts/knl/test49.0.998.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L650-800 650 800 0.0
#python3 ../scripts/knl/test49.0.999.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L900-1050 900 1050 0.0
#
##### SGS
## this is not very good script though, from sh1.SGS_sift100M_find_L
#../scripts/test46.find_L_simple_search_large_graph.py ~/data sift100m SGS_find_L1600 90 1600 0.0

set +x


