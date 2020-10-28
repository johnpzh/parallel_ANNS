#!/usr/local/bin/zsh

set -x
#### SIFT100M
#python3 ../scripts/test53.increase_X_SV3_T8.py ~/data increase_X_T8_L90-95 90 95 0.9
#python3 ../scripts/test53.increase_X_SV3_T8.py ~/data increase_X_T8_L95-100 95 100 0.99
#python3 ../scripts/test53.increase_X_SV3_T8.py ~/data increase_X_T8_L180-190 180 190 0.999

#### SIFT1M
python3 ../scripts/test56.half_X_SV3_T8.py ~/data half_X_T8_L90-91 90 91 0.9
python3 ../scripts/test56.half_X_SV3_T8.py ~/data half_X_T8_L90-100 90 100 0.99 0.999

set +x


