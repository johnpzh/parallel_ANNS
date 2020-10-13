#!/bin/bash

python3 ../scripts/test43.simple_v3_ranged_L.py ~/data SV3_T2_L90-100 2 90 100 1 1 4
python3 ../scripts/test43.simple_v3_ranged_L.py ~/data SV3_T2_L100-200 2 100 200 4 2 16
python3 ../scripts/test43.simple_v3_ranged_L.py ~/data SV3_T2_L200-400 2 200 400 8 2 20
python3 ../scripts/test43.simple_v3_ranged_L.py ~/data SV3_T2_L400-500 2 400 500 16 2 32