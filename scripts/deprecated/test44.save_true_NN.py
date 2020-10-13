#! python3
import os
import sys
import subprocess

if len(sys.argv) != 2:
    print(f"{sys.argv[0]} <data_dir>")
    # print(f"{sys.argv[0]} <data_dir> <tag> <num_t> <L_low> <L_up>")
    exit()

base_dir = sys.argv[1]
# tag = sys.argv[2]
# num_t = int(sys.argv[3])
# L_lower = int(sys.argv[4])
# L_upper = int(sys.argv[5])

env_vars = os.environ
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,1,0"
bin="./save_true_nearest_neighbors"

#### DEEP10M
data_dir = base_dir + "/deep1b"
data_name = "deep100M"
# label = F"{tag}.deep10M"
# raw_file = F"output.{label}.raw.txt"

# subprocess.run(F':> {raw_file}', shell=True, check=True)
# L_min=L_lower
# L_max=L_upper
command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
          F"100 {data_dir}/{data_name}.true-100_NN.q-10000.binary"
subprocess.run(command, env=env_vars, shell=True, check=True)
#
# for L in range(L_lower, L_upper):
#     for X in range(X_lower, L + 5):
#         command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
#                   F"{L} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.q-10000.binary {num_t} {L} {X} " \
#                   F"| tee -a {raw_file}"
#         subprocess.run(command, env=env_vars, shell=True, check=True)

# rows_file = F"output.{label}.rows.txt"
# table_file = F"output.{label}.table.txt"
# selected_file = F"output.{label}.table.selected.txt"
# subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 12 13 15 1', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 2 0', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)
