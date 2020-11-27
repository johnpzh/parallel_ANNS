#!/usr/local/bin/zsh

bin="python3 ../scripts/skylake/test47.simple_v3_large_graph_ranged_L.py"
data_path="/scratch/zpeng"

set -x
#eval ${bin} ${data_path} SV3_T4_L90-100 4 90 100 2 1 2
#eval ${bin} ${data_path} SV3_T4_L100-400 4 100 400 32 2 64
#eval ${bin} ${data_path} SV3_T4_L400-800 4 400 800 64 2 128
#eval ${bin} ${data_path} SV3_T4_L800-1600 4 800 1600 128 2 256
##${bin} ${data_path} SV3_T4_L1600-3200 4 1600 3200 64 2 128
##${bin} ${data_path} SV3_T4_L3200-5000 4 3200 5000 128 2 256

eval ${bin} ${data_path} SV3_T2_L90-100 2 90 100 2 1 2
eval ${bin} ${data_path} SV3_T2_L100-400 2 100 400 32 2 64
eval ${bin} ${data_path} SV3_T2_L400-800 2 400 800 64 2 128
eval ${bin} ${data_path} SV3_T2_L800-1600 2 800 1600 128 2 256
eval ${bin} ${data_path} SV3_T2_L1600-2400 2 1600 2400 256 2 512
#${bin} ${data_path} SV3_T4_L3200-5000 4 3200 5000 128 2 256

#set -x
#${bin} ${data_path} SV3_T8_L90-100 8 90 100 2 1 2
#${bin} ${data_path} SV3_T8_L100-400 8 100 400 32 2 64
#${bin} ${data_path} SV3_T8_L400-800 8 400 800 64 2 128
##${bin} ${data_path} SV3_T8_L800-1600 8 800 1600 32 2 64
##${bin} ${data_path} SV3_T8_L1600-3200 8 1600 3200 64 2 128
##${bin} ${data_path} SV3_T8_L3200-5000 8 3200 5000 128 2 256
set +x


