#!/usr/local/bin/zsh

set -x

python3 ../scripts/knl/test49.0.90-0.98.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L90-250 90 250 0.0
python3 ../scripts/knl/test49.0.990-0.995.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L250-500 250 500 0.0
python3 ../scripts/knl/test49.0.996.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L400-550 400 550 0.0
python3 ../scripts/knl/test49.0.997.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L500-650 500 650 0.0
python3 ../scripts/knl/test49.0.998.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L650-800 650 800 0.0
python3 ../scripts/knl/test49.0.999.find_L_SV3_T1_large_graph.py ~/data SV3_T1_find_L900-1050 900 1050 0.0

set +x


