#PBS -N curve_middle-M-4
#PBS -l walltime=4:00:00
#PBS -l nodes=1:ppn=40
#PBS -j oe
#PBS -m abe
#PBS -A PGS0218

# qsub -I -X -l nodes=1:ppn=40 -l walltime=1:00:00 -A PGS0218 -m abe

cd /users/PGS0218/zpeng01/pppp/clion/panns_mac/cmake-build-release || exit

../scripts/curve_engine.sh ../scripts/curve.pitzer.profile_para_top_m_search.middle-M-4.sh curve_middle-M-4_2 9 11 4 /users/PGS0218/zpeng01/data 2