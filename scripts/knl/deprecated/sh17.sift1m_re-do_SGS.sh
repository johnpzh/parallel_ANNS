#!/usr/local/bin/zsh

set -x

#### SIFT1M 1T

../scripts/test50.find_L_SV3_T1.py ~/data sift1m PSS_T1_L90-410 90 420 0

#num_t=1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L90-100 ${num_t} 90 100 1 0 0 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L115 ${num_t} 115 115 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L159 ${num_t} 159 159 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L167 ${num_t} 167 167 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L179 ${num_t} 179 179 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L189 ${num_t} 189 189 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L201 ${num_t} 201 201 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L216 ${num_t} 216 216 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L240 ${num_t} 240 240 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L268 ${num_t} 268 268 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L317 ${num_t} 317 317 1 0 1 1
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L405 ${num_t} 405 405 1 0 1 1

#### SIFT1M

 ../scripts/test48.find_L_simple_search.py ~/data sift1m SGS_L90-440 90 440 0

##### SIFT1M 2T
#num_t=2
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L90-100 ${num_t} 90 100 2 20 50 2
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L100-102 ${num_t} 100 102 2 50 100 3
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L107-127 ${num_t} 107 127 2 90 120 3
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L132-134 ${num_t} 132 134 2 120 140 2
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L140-142 ${num_t} 140 142 2 120 150 2
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L190-192 ${num_t} 190 192 2 180 200 2
#../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L220-226 ${num_t} 220 226 3 210 243 3

set +x


