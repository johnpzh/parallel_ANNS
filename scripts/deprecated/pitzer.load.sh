#PBS -N dist_threshold
#PBS -l walltime=48:00:00
#PBS -l nodes=1:ppn=40
#PBS -j oe
#PBS -m abe
#PBS -A PGS0218

# qsub -I -l nodes=1:ppn=40 -l walltime=1:00:00 -A PGS0218 -m abe

cd /users/PGS0218/zpeng01/pppp/clion/panns_mac/cmake-build-release || exit

../scripts/performance_engine.sh ../scripts/profile.pitzer.profile_para_top_m.dist_threshold.sh dist_threshold 3 4 11 9 4 /users/PGS0218/zpeng01/data