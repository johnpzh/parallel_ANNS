#!/usr/local/bin/zsh

set -x

#### SIFT1M 4T
num_t=4
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L90-100 ${num_t} 90 100 2 10 30 2
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L100-102 ${num_t} 100 102 2 30 100 3
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L113-115 ${num_t} 113 115 2 90 120 3

#### SIFT1M 2T
num_t=2
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L90-100 ${num_t} 90 100 2 20 50 2
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L100-102 ${num_t} 100 102 2 50 100 3
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L107-127 ${num_t} 107 127 2 90 120 3
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L132-134 ${num_t} 132 134 2 120 140 2
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L140-142 ${num_t} 140 142 2 120 150 2
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L190-192 ${num_t} 190 192 2 180 200 2
../scripts/test43.simple_v3_ranged_L.py ~/data sift1m PSS_T${num_t}_L220-226 ${num_t} 220 226 3 210 243 3

set +x


