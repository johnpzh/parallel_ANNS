#PBS -N curve_selecting_lth
#PBS -l walltime=03:00:00
#PBS -l nodes=1:ppn=40
#PBS -j oe
#PBS -m abe
#PBS -A PGS0218

# qsub -I -X -l nodes=1:ppn=40 -l walltime=1:00:00 -A PGS0218 -m abe

cd /users/PGS0218/zpeng01/pppp/clion/panns_mac/cmake-build-release || exit

../scripts/curve_engine.sh ../scripts/curve.pitzer.profile_para_top_m_search.selecting_lth.sh curve_selecting_lth 9 11 4 /users/PGS0218/zpeng01/data 2