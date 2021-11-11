#! python3
import os
import sys
import subprocess

if len(sys.argv) < 8:
    print(f"{sys.argv[0]} <app> <data_dir> <data> <tag> <ef_low> <ef_up> <P_target> [<P_target> ...]")
    exit()

app = sys.argv[1]
base_dir = sys.argv[2]
data = sys.argv[3]
tag = sys.argv[4]
ef_lower = int(sys.argv[5])
ef_upper = int(sys.argv[6])
base_loc_P_target = 7
targets = [sys.argv[i] for i in range(base_loc_P_target, len(sys.argv))]
P_level = " ".join(targets)

env_vars = os.environ
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,1,0"
# bin=F"numactl -m 0 ./{app}"
bin=F"./{app}"

if data == "sift1m":
    data_dir = base_dir + "/sift1m"
    data_name = "sift"
    num_v = 1
    dim = 128
    num_q = 10000
elif data == "gist1m":
    data_dir = base_dir + "/gist1m"
    data_name = "gist"
    num_v = 1
    dim = 960
    num_q = 1000
elif data == "deep10m":
    data_dir = base_dir + "/deep1b"
    data_name = "deep10M"
    num_v = 10
    dim = 96
    num_q = 10000
elif data == "sift100m":
    data_dir = base_dir + "/sift1b"
    data_name = "sift100M"
    num_v = 100
    dim = 128
    num_q = 10000
elif data == "deep100m":
    data_dir = base_dir + "/deep1b"
    data_name = "deep100M"
    num_v = 100
    dim = 96
    num_q = 10000
elif data == "deep1m":
    data_dir = base_dir + "/deep1b"
    data_name = "deep1M"
    num_v = 1
    dim = 96
    num_q = 10000
elif data == "deep1b":
    data_dir = base_dir + "/deep1b"
    data_name = "deep1B"
    num_v = 1000
    dim = 96
    num_q = 10000
elif data == "sift1b":
    data_dir = base_dir + "/sift1b"
    data_name = "sift1B"
    num_v = 1000
    dim = 128
    num_q = 10000
else:
    print(F"Error: data {data} is unknown.")
    exit()

label = F"{data}.{tag}"
raw_file = F"output.{label}.raw.txt"

subprocess.run(F':> {raw_file}', shell=True, check=True)
command = F"{bin} {data_dir}/{data_name}.hnsw {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.true-100_NN.v2.binary " \
          F"{num_v} {dim} {num_q} {ef_lower} " \
          F"{ef_upper} {P_level} " \
          F"| tee -a {raw_file}"
print(command)
subprocess.run(command, env=env_vars, shell=True, check=True)

rows_file = F"output.{label}.rows.txt"
table_file = F"output.{label}.table.txt"
subprocess.run(F"python3 ../scripts/output_surrounding.py {raw_file} {rows_file}", shell=True, check=True)
subprocess.run(F"python3 ../scripts/output_format.py {rows_file} {table_file} 0:8", shell=True, check=True)

