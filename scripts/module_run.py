#! python3
# import os
import sys
import subprocess

if len(sys.argv) < 4:
    print(F'{sys.argv[0]} <bin> <sift|gist|deep10M> <data_diretory> [additional]')
    exit()
elif sys.argv[2] not in ['sift', 'gist', 'deep10M']:
    print(F'Error: does not support data mode {sys.argv[2]} (support only sift|gist|deep10M)')
    exit()

# data_path="/scratch/zpeng"
data_path=sys.argv[3]
# data_path="/data/zpeng"

sift_args = F'{data_path}/sift1m/sift_base.fvecs {data_path}/sift1m/sift_query.fvecs ' \
            F'{data_path}/sift1m/sift.nsg 200 200 output.ivecs'
gist_args = F'{data_path}/gist1m/gist_base.fvecs {data_path}/gist1m/gist_query.fvecs ' \
            F'{data_path}/gist1m/gist.nsg 400 400 output.ivecs'
deep10M_args = F'{data_path}/deep1b/deep10M_base.fvecs {data_path}/deep1b/deep10M_query.fvecs ' \
            F'{data_path}/deep1b/deep10M.nsg 400 400 output.ivecs'

command = [sys.argv[1]]

if sys.argv[2] == 'sift':
    command.append(sift_args)
elif sys.argv[2] == 'gist':
    command.append(gist_args)
elif sys.argv[2] == 'deep10M':
    command.append(deep10M_args)
else:
    print(F'Error: data mode {sys.argv[2]} not found.')
    exit()
command.extend(sys.argv[4:])

# os.system(' '.join(command))
# For ICC CPU Affinity
# 0,0 for Pitzer; 1,0 for KNL
# subprocess.run('export KMP_AFFINITY="granularity=fine,compact,0,0"; ' + \
subprocess.run('export KMP_AFFINITY="granularity=fine,compact,1,0"; ' + \
# subprocess.run('export KMP_AFFINITY="verbose,granularity=fine,compact,1,0"; ' + \
# subprocess.run('export KMP_AFFINITY="verbose,granularity=core,compact,0,0"; ' + \
# subprocess.run('export KMP_AFFINITY="verbose,granularity=core,compact,1,0"; ' + \
# subprocess.run('export KMP_AFFINITY="verbose,granularity=core,scatter,0,0"; ' + \
               ' '.join(command), shell=True, check=True)
# subprocess.run(' '.join(command), shell=True, check=True)
# result = subprocess.run(' '.join(command), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# print(result.stdout)
# print(result.stderr)