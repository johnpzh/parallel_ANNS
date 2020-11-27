#!/usr/local/bin/zsh

set -x
bin="../scripts/test47.simple_v3_large_graph_ranged_L.py"
#### SIFT100M 8T
num_t=8
#${bin} ~/data sift100m PSS_T${num_t}_L91-100 ${num_t} 92 100 2 10 30 2
#${bin} ~/data sift100m PSS_T${num_t}_L100-102 ${num_t} 100 102 2 30 100 3
${bin} ~/data sift100m PSS_T${num_t}_L180-188 ${num_t} 180 192 4 126 138 3
#
##### SIFT100M 4T
#num_t=4
#${bin} ~/data sift100m PSS_T${num_t}_L94-98 ${num_t} 94 98 2 10 30 2
#${bin} ~/data sift100m PSS_T${num_t}_L98-100 ${num_t} 98 100 2 40 100 3
#${bin} ~/data sift100m PSS_T${num_t}_L130-150 ${num_t} 130 150 3 100 130 3
#${bin} ~/data sift100m PSS_T${num_t}_L178-180 ${num_t} 178 180 2 130 150 2
#${bin} ~/data sift100m PSS_T${num_t}_L226-228 ${num_t} 226 228 2 160 200 3
#${bin} ~/data sift100m PSS_T${num_t}_L274-276 ${num_t} 274 276 2 250 260 2

#### SIFT100M 2T
num_t=2
${bin} ~/data sift100m PSS_T${num_t}_L96-100 ${num_t} 96 100 2 20 50 3
${bin} ~/data sift100m PSS_T${num_t}_L100-102 ${num_t} 100 102 2 60 100 3
${bin} ~/data sift100m PSS_T${num_t}_L146-148 ${num_t} 146 148 2 120 130 2
${bin} ~/data sift100m PSS_T${num_t}_L210-212 ${num_t} 210 212 2 170 180 2
${bin} ~/data sift100m PSS_T${num_t}_L226-228 ${num_t} 226 228 2 190 200 2
${bin} ~/data sift100m PSS_T${num_t}_L250-260 ${num_t} 250 260 2 210 220 2
${bin} ~/data sift100m PSS_T${num_t}_L290-292 ${num_t} 290 292 2 210 260 6
${bin} ~/data sift100m PSS_T${num_t}_L386-388 ${num_t} 386 388 2 270 340 7
${bin} ~/data sift100m PSS_T${num_t}_L462-464 ${num_t} 462 464 2 380 390 2
${bin} ~/data sift100m PSS_T${num_t}_L590-592 ${num_t} 590 592 2 500 520 3

#### SIFT100M 1T
# for backup
# from sh2.PSS_sift100M_T1_find_L
python3 ../scripts/knl/test49.0.90-0.98.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L90-250 90 250 0.0
python3 ../scripts/knl/test49.0.990-0.995.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L250-500 250 500 0.0
python3 ../scripts/knl/test49.0.996.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L400-550 400 550 0.0
python3 ../scripts/knl/test49.0.997.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L500-650 500 650 0.0
python3 ../scripts/knl/test49.0.998.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L650-800 650 800 0.0
python3 ../scripts/knl/test49.0.999.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L900-1050 900 1050 0.0

#### SGS
# this is not very good script though, from sh1.SGS_sift100M_find_L
../scripts/test46.find_L_simple_search_large_graph.py ~/data sift100m SGS_find_L1600 90 1600 0.0

set +x


