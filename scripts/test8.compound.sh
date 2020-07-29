#! /usr/bin/bash
####! /users/PGS0218/zpeng01/local/bin/zsh
####! /usr/local/bin/zsh
####! /bin/bash

../scripts/performance_engine.sh ../scripts/test7.middle_m.sh middle_m 3 4 11 13 14 4 /data/zpeng

../scripts/performance_engine.sh ../scripts/test4.select_mth.sh gather_m 3 4 11 13 14 4 /data/zpeng

../scripts/performance_engine.sh ../scripts/test7.middle_m.large_L.sh middle_m_large_L 3 4 11 13 14 4 /data/zpeng

../scripts/performance_engine.sh ../scripts/test4.select_mth.large_L.sh gather_m_large_L 3 4 11 13 14 4 /data/zpeng