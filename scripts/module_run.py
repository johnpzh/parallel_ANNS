#! python3
import os
import sys

if len(sys.argv) < 3:
    print(F'{sys.argv[0]} <bin> <sift|gist|deep> [additional]')
    exit(-1)
elif sys.argv[2] not in ['sift', 'gist', 'deep']:
    print(F'Error: does not support data mode {sys.argv[2]} (support only sift|gist|deep)')
    exit(-1)

sift_args = '/scratch/zpeng/sift1m/sift_base.fvecs /scratch/zpeng/sift1m/sift_query.fvecs ' \
            '/scratch/zpeng/sift1m/sift.nsg 200 200 output.ivecs'
gist_args = '/scratch/zpeng/gist1m/gist_base.fvecs /scratch/zpeng/gist1m/gist_query.fvecs ' \
            '/scratch/zpeng/gist1m/gist.nsg 400 400 output.ivecs'
deep_args = '/scratch/zpeng/deep1b/deep10M_base.fvecs /scratch/zpeng/deep1b/deep10M_query.fvecs ' \
            '/scratch/zpeng/deep1b/deep10M.nsg 400 400 output.ivecs'

command = [sys.argv[1]]

if sys.argv[2] == 'sift':
    command.append(sift_args)
elif sys.argv[2] == 'gist':
    command.append(gist_args)
elif sys.argv[2] == 'deep':
    command.append(deep_args)
else:
    print(F'Error: data mode {sys.argv[2]} not found.')
    exit(-1)
command.extend(sys.argv[3:])

os.system(' '.join(command))
