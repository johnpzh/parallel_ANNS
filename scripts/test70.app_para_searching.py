#! python3
import os
import sys
import subprocess

if len(sys.argv) != 7:
    print(F"{sys.argv[0]} <app> <data_dir> <data> "
          F"<tag> <num_t> <L>")
    # print(f"{sys.argv[0]} <data_dir> <tag>")
    exit()

app = sys.argv[1]
base_dir = sys.argv[2]
data = sys.argv[3]
tag = sys.argv[4]
num_t = int(sys.argv[5])
L = int(sys.argv[6])
# K = int(sys.argv[7])

env_vars = os.environ
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,1,0"
# bin=F"numactl -m 0 ./{app}"
bin=F"./{app}"

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
elif data == "deep1m":
    data_dir = base_dir + "/deep1b"
    data_name = "deep1M"
elif data == "deep1b":
    data_dir = base_dir + "/deep1b"
    data_name = "deep1B"
elif data == "sift1b":
    data_dir = base_dir + "/sift1b"
    data_name = "sift1B"
else:
    print(F"Error: data {data} is unknown.")
    exit()

label = F"{data}.{tag}"
raw_file = F"output.{label}.raw.txt"

subprocess.run(F':> {raw_file}', shell=True, check=True)
command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
          F"{L} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.v2.binary " \
          F"{num_t} " \
          F"| tee -a {raw_file}"
subprocess.run(command, env=env_vars, shell=True, check=True)

rows_file = F"output.{label}.rows.txt"
table_file = F"output.{label}.table.txt"
selected_file = F"output.{label}.selected.txt"
subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 9 12 13 1', shell=True, check=True)
subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 4 0', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)

