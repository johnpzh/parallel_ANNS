#!/usr/local/bin/zsh

bin="python3 ../scripts/test58.increase_X_SV3_large_graph.py"
num_t=8
set -x

eval ${bin} ~/data sift100m increase_X_T${num_t}_L180-188 ${num_t} 180 188 0.999
eval ${bin} ~/data deep100m increase_X_T${num_t}_L110-113 ${num_t} 410 418 0.999

set +x


