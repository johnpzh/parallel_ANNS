#! /bin/bash
set -x
#../scripts/module_run.py ./app_seq_searching sift
#mv output.ivecs /scratch/zpeng/sift1m/output.ivecs
cd ../cmake-build-release || exit

python3 ../scripts/module_run.py ./app_seq_searching gist
cp output.ivecs /scratch/zpeng/gist1m/output.ivecs
md5sum output.ivecs > output.md5sum.gist1M.txt

python3 ../scripts/module_run.py ./app_seq_searching deep
cp output.ivecs /scratch/zpeng/deep1b/output.ivecs
md5sum output.ivecs > output.md5sum.deep10M.txt


set +x