#! python3
import os
import sys
import subprocess

if len(sys.argv) < 11:
    print(f"{sys.argv[0]} <app> <data_dir> <data> "
          f"<tag> <num_t> <L_master_low> <L_master_up> "
          f"<L_local> <X> <P_target> [<P_target> ...]")
    # print(f"{sys.argv[0]} <data_dir> <tag>")
    exit()

app = sys.argv[1]
base_dir = sys.argv[2]
data = sys.argv[3]
tag = sys.argv[4]
num_t = int(sys.argv[5])
# L_master = int(sys.argv[6])
L_master_lower = int(sys.argv[6])
L_master_upper = int(sys.argv[7])
L_local = int(sys.argv[8])
X = int(sys.argv[9])
base_loc_P_target = 10
targets = [sys.argv[i] for i in range(base_loc_P_target, len(sys.argv))]
P_level = " ".join(targets)

env_vars = os.environ
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,1,0"
# bin="numactl -m 0 ./profile_find_L_para_single_query_search_simple_v3"
bin=F"numactl -p 0 ./{app}"

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
elif data == "sift1b":
    data_dir = base_dir + "/sift1b"
    data_name = "sift1B"
elif data == "deep1b":
    data_dir = base_dir + "/deep1b"
    data_name = "deep1B"
else:
    print(F"Error: data {data} is unknown.")
    exit()

label = F"{data}.{tag}"
raw_file = F"output.{label}.raw.txt"

subprocess.run(F':> {raw_file}', shell=True, check=True)
command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
          F"100 output.ivecs {data_dir}/{data_name}.true-100_NN.v2.binary {num_t} " \
          F"{L_master_lower} {L_master_upper} {L_local} {X} {P_level} " \
          F"| tee -a {raw_file}"
subprocess.run(command, env=env_vars, shell=True, check=True)

rows_file = F"output.{label}.rows.txt"
table_file = F"output.{label}.table.txt"
subprocess.run(F"python3 ../scripts/output_surrounding.py {raw_file} {rows_file}", shell=True, check=True)
subprocess.run(F"python3 ../scripts/output_format.py {rows_file} {table_file} 0:16", shell=True, check=True)