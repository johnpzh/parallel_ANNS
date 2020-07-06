#PBS -N L_T100_subsearch
#PBS -l walltime=01:00:00
#PBS -l nodes=1:ppn=40
#PBS -m abe
#PBS -A PGS0218
#PBS -j oe

########!/usr/bin/bash
# qsub -I -X -l nodes=1:ppn=40 -l walltime=1:00:00 -A PGS0218 -m abe

cd /users/PGS0218/zpeng01/pppp/clion/panns_mac/cmake-build-release

../scripts/performance_engine.sh ../scripts/profile.pitzer.profile_para_top_m.subsearch.sh L_T100_subsearch 3 4 11 9 4 /users/PGS0218/zpeng01/data
