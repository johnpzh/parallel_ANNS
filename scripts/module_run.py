#! python3
# import os
import sys
import subprocess

if len(sys.argv) < 3:
    print(F'{sys.argv[0]} <bin> <sift|gist|deep10M> [additional]')
    exit()
elif sys.argv[2] not in ['sift', 'gist', 'deep10M']:
    print(F'Error: does not support data mode {sys.argv[2]} (support only sift|gist|deep10M)')
    exit()

sift_args = '/scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs ' \
            '/scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs'
gist_args = '/scratch/zpeng/gist1m/gist_base.fvecs /scratch/zpeng/gist1m/gist_query.fvecs ' \
            '/scratch/zpeng/gist1m/gist.nsg 400 400 output.ivecs'
deep10M_args = '/scratch/zpeng/deep1b/deep10M_base.fvecs /scratch/zpeng/deep1b/deep10M_query.fvecs ' \
            '/scratch/zpeng/deep1b/deep10M.nsg 400 400 output.ivecs'

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
command.extend(sys.argv[3:])

# os.system(' '.join(command))
# For ICC CPU Affinity
subprocess.run('export KMP_AFFINITY="granularity=fine,compact,1,0"; ' + \
               ' '.join(command), shell=True, check=True)
# subprocess.run(' '.join(command), shell=True, check=True)
# result = subprocess.run(' '.join(command), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# print(result.stdout)
# print(result.stderr)