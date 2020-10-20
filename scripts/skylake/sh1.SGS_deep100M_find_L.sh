#!/usr/local/bin/zsh

set -x

python3 ../scripts/skylake/test46.0.90-0.98.find_L_simple_search_large_graph.py /scratch/zpeng SGS_find_L100-500 100 500 0.0
python3 ../scripts/skylake/test46.0.990-0.993.find_L_simple_search_large_graph.py /scratch/zpeng SGS_find_L500-1000 500 1000 0.0
python3 ../scripts/skylake/test46.0.994-0.996.find_L_simple_search_large_graph.py /scratch/zpeng SGS_find_L1000-1500 1000 1500 0.0
python3 ../scripts/skylake/test46.0.997.find_L_simple_search_large_graph.py /scratch/zpeng SGS_find_L1500-2000 1500 2000 0.0
python3 ../scripts/skylake/test46.0.998.find_L_simple_search_large_graph.py /scratch/zpeng SGS_find_L2000-2500 2000 2500 0.0
python3 ../scripts/skylake/test46.0.999.find_L_simple_search_large_graph.py /scratch/zpeng SGS_find_L4000-4500 4000 4500 0.0

set +x


