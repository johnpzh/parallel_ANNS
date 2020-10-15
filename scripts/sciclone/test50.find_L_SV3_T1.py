#! python3
import os
import sys
import subprocess

if len(sys.argv) != 6:
    print(f"{sys.argv[0]} <data_dir> <tag> <L_low> <L_up> <P_target>")
    # print(f"{sys.argv[0]} <data_dir> <tag>")
    exit()

base_dir = sys.argv[1]
tag = sys.argv[2]
# num_t = int(sys.argv[3])
L_lower = int(sys.argv[3])
L_upper = int(sys.argv[4])
P_level = float(sys.argv[5])

env_vars = os.environ
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,0,0"
bin="numactl -m 0 ./profile_find_L_para_single_query_search_simple_v3"

#### SIFT100M
data_dir = base_dir + "/sift1b"
data_name = "sift100M"
label = F"{tag}.{data_name}"
raw_file = F"output.{label}.raw.txt"

subprocess.run(F':> {raw_file}', shell=True, check=True)
for P_level in [0.90, 0.91, 0.92, 0.93,
                0.94, 0.95, 0.96, 0.97,
                0.98, 0.99,
                0.991, 0.992, 0.993, 0.994,
                0.995, 0.996, 0.997, 0.998,
                0.999]:
    command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
              F"{L_lower} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.v2.binary" \
              F"1 {L_upper} {P_level} " \
              F"| tee -a {raw_file}"
    subprocess.run(command, env=env_vars, shell=True, check=True)

rows_file = F"output.{label}.rows.txt"
table_file = F"output.{label}.table.txt"
subprocess.run(F"python3 ../scripts/output_surrounding.py {raw_file} {rows_file}", shell=True, check=True)
subprocess.run(F"python3 ../scripts/output_format.py {rows_file} {table_file} 0:7", shell=True, check=True)

#### DEEP100M
data_dir = base_dir + "/deep1b"
data_name = "deep100M"
label = F"{tag}.{data_name}"
raw_file = F"output.{label}.raw.txt"

subprocess.run(F':> {raw_file}', shell=True, check=True)
for P_level in [0.90, 0.91, 0.92, 0.93,
                0.94, 0.95, 0.96, 0.97,
                0.98, 0.99,
                0.991, 0.992, 0.993, 0.994,
                0.995, 0.996, 0.997, 0.998,
                0.999]:
    command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
              F"{L_lower} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.v2.binary" \
              F"1 {L_upper} {P_level} " \
              F"| tee -a {raw_file}"
    subprocess.run(command, env=env_vars, shell=True, check=True)

rows_file = F"output.{label}.rows.txt"
table_file = F"output.{label}.table.txt"
subprocess.run(F"python3 ../scripts/output_surrounding.py {raw_file} {rows_file}", shell=True, check=True)
subprocess.run(F"python3 ../scripts/output_format.py {rows_file} {table_file} 0:7", shell=True, check=True)

# #### SIFT1M
# data_dir = base_dir + "/sift1m"
# data_name = "sift"
# label = F"{tag}"
# raw_file = F"output.{label}.raw.txt"
#
# subprocess.run(F':> {raw_file}', shell=True, check=True)
# L = 200
# X = 1
# command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
#           F"{L} 200 output.ivecs {data_dir}/{data_name}.true-100_NN.q-10000.binary {num_t} {L} {X} " \
#           F"| tee -a {raw_file}"
# subprocess.run(command, env=env_vars, shell=True, check=True)

# for L in range(L_lower, L_upper + 1):
#     for X in range(X_lower, L + 5):
#         command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
#                   F"{L} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.q-10000.binary {num_t} {L} {X} " \
#                   F"| tee -a {raw_file}"
#         subprocess.run(command, env=env_vars, shell=True, check=True)
#
# rows_file = F"output.{label}.rows.txt"
# table_file = F"output.{label}.table.txt"
# selected_file = F"output.{label}.table.selected.txt"
# subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 12 13 15 1', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 2 0', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)

# #### GIST1M
# data_dir = base_dir + "/gist1m"
# data_name = "gist"
# label = F"{tag}.gist1M"
# raw_file = F"output.{label}.raw.txt"
#
# subprocess.run(F':> {raw_file}', shell=True, check=True)
#
# for L in range(L_lower, L_upper + 1):
#     for X in range(X_lower, L + 5):
#         command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
#                   F"{L} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.q-1000.binary {num_t} {L} {X} " \
#                   F"| tee -a {raw_file}"
#         subprocess.run(command, env=env_vars, shell=True, check=True)
#
# rows_file = F"output.{label}.rows.txt"
# table_file = F"output.{label}.table.txt"
# selected_file = F"output.{label}.table.selected.txt"
# subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 12 13 15 1', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 2 0', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)

# #### DEEP10M
# data_dir = base_dir + "/deep1b"
# data_name = "deep10M"
# label = F"{tag}.deep10M"
# raw_file = F"output.{label}.raw.txt"
#
# subprocess.run(F':> {raw_file}', shell=True, check=True)
#
# for L in range(L_lower, L_upper + 1):
#     for X in range(X_lower, L + 5):
#         command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
#                   F"{L} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.q-10000.binary {num_t} {L} {X} " \
#                   F"| tee -a {raw_file}"
#         subprocess.run(command, env=env_vars, shell=True, check=True)
#
# rows_file = F"output.{label}.rows.txt"
# table_file = F"output.{label}.table.txt"
# selected_file = F"output.{label}.table.selected.txt"
# subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 12 13 15 1', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 2 0', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)
