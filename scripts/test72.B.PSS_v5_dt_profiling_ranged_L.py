#! python3
import os
import sys
import subprocess

if len(sys.argv) != 18:
    print(F"{sys.argv[0]} <app> <data_dir> <data> "
          F"<tag> <num_t> <L_master_low> <L_master_up> "
          F"<L_master_step> <L_local_low> <L_local_up> <L_local_step> "
          F"<X_low> <X_up> <X_step> <I_thresh_low> "
          F"<I_thresh_up> <I_thresh_step>")
    # print(f"{sys.argv[0]} <data_dir> <tag>")
    exit()

app = sys.argv[1]
base_dir = sys.argv[2]
data = sys.argv[3]
tag = sys.argv[4]
num_t = int(sys.argv[5])
L_master_low = int(sys.argv[6])
L_master_up = int(sys.argv[7])
L_master_step = int(sys.argv[8])
L_local_low = int(sys.argv[9])
L_local_up = int(sys.argv[10])
L_local_step = int(sys.argv[11])
X_lower = int(sys.argv[12])
X_upper = int(sys.argv[13])
X_step = int(sys.argv[14])
I_thresh_low = int(sys.argv[15])
I_thresh_up = int(sys.argv[16])
I_thresh_step = int(sys.argv[17])

env_vars = os.environ
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,1,16"
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
          F"100 output.ivecs {data_dir}/{data_name}.true-100_NN.v2.binary {num_t} " \
          F"{L_master_low} {L_master_up} {L_master_step} {L_local_low} " \
          F"{L_local_up} {L_local_step} {X_lower} {X_upper} " \
          F"{X_step} {I_thresh_low} {I_thresh_up} {I_thresh_step}" \
          F"| tee -a {raw_file}"
subprocess.run(command, env=env_vars, shell=True, check=True)

rows_file = F"output.{label}.rows.txt"
table_file = F"output.{label}.table.txt"
selected_file = F"output.{label}.selected.txt"
subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 9 12 13 1 14 15 16 17 18:35', shell=True, check=True)
subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 1 0', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)

