#! python3
import os
import sys
import subprocess

if len(sys.argv) < 9:
    print(f"{sys.argv[0]} <app> <data_dir> <data> <tag> <L_low> <L_up> <num_t> <P_target> [<P_target> ...]")
    # print(f"{sys.argv[0]} <data_dir> <tag>")
    exit()

app = sys.argv[1]
base_dir = sys.argv[2]
data = sys.argv[3]
tag = sys.argv[4]
L_lower = int(sys.argv[5])
L_upper = int(sys.argv[6])
num_t = int(sys.argv[7])
base_loc_P_target = 8
targets = [sys.argv[i] for i in range(base_loc_P_target, len(sys.argv))]
P_level = " ".join(targets)

env_vars = os.environ
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,1,0"
bin=F"numactl -p 0 ./{app}"
# bin=F"numactl -m 0 ./{app}"
# bin="numactl -m 0 ./profile_find_L_seq_single_query_simple_search"

if data == "sift1m":
    data_dir = base_dir + "/sift1m"
    data_name = "sift"
elif data == "gist1m":
    data_dir = base_dir + "/gist1m"
    data_name = "gist"
elif data == "deep10m":
    data_dir = base_dir + "/deep1b"
    data_name = "deep10M"
elif data == "sift100m":
    data_dir = base_dir + "/sift1b"
    data_name = "sift100M"
elif data == "deep100m":
    data_dir = base_dir + "/deep1b"
    data_name = "deep100M"
else:
    print(F"Error: data {data} is unknown.")
    exit()

label = F"{data}.{tag}"
raw_file = F"output.{label}.raw.txt"

subprocess.run(F':> {raw_file}', shell=True, check=True)
command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
          F"{L_lower} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.v2.binary " \
          F"{L_upper} {num_t} {P_level} " \
          F"| tee -a {raw_file}"
subprocess.run(command, env=env_vars, shell=True, check=True)

rows_file = F"output.{label}.rows.txt"
table_file = F"output.{label}.table.txt"
subprocess.run(F"python3 ../scripts/output_surrounding.py {raw_file} {rows_file}", shell=True, check=True)
subprocess.run(F"python3 ../scripts/output_format.py {rows_file} {table_file} 0:9", shell=True, check=True)

