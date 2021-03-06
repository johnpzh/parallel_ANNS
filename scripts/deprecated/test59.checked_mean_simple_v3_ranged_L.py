#! python3
import os
import sys
import subprocess

if len(sys.argv) != 11:
    print(f"{sys.argv[0]} <data_dir> <data> <tag> <num_t> <L_low> <L_up> <L_step> <X_low> <X_up> <X_step>")
    # print(f"{sys.argv[0]} <data_dir> <tag>")
    exit()

base_dir = sys.argv[1]
data = sys.argv[2]
tag = sys.argv[3]
num_t = int(sys.argv[4])
L_lower = int(sys.argv[5])
L_upper = int(sys.argv[6])
L_step = int(sys.argv[7])
X_lower = int(sys.argv[8])
X_upper = int(sys.argv[9])
X_step = int(sys.argv[10])

env_vars = os.environ
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,1,0"
# bin="./profile_para_single_query_search_simple_v3_ranged_num_checked"
# bin="./profile_para_single_query_search_simple_v3_ranged"
bin="./A"
# bin="./B"

if data == "sift1m":
    data_dir = base_dir + "/sift1m"
    data_name = "sift"
elif data == "gist1m":
    data_dir = base_dir + "/gist1m"
    data_name = "gist"
elif data == "deep10m":
    data_dir = base_dir + "/deep1b"
    data_name = "deep10M"
else:
    print(F"Error: data {data} is unknown.")
    exit()

label = F"{tag}.{data_name}"
raw_file = F"output.{label}.raw.txt"

subprocess.run(F':> {raw_file}', shell=True, check=True)
L_min=L_lower
L_max=L_upper
command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
          F"{L_min} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.v2.binary " \
          F"{num_t} {L_max} {L_step} {X_lower} {X_upper} {X_step} " \
          F"| tee -a {raw_file}"
subprocess.run(command, env=env_vars, shell=True, check=True)

rows_file = F"output.{label}.rows.txt"
table_file = F"output.{label}.table.txt"
selected_file = F"output.{label}.table.selected.txt"
subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 9 12 13 1 15 17 18', shell=True, check=True)
subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 1 0', shell=True, check=True)
subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)

# #### GIST1M
# data_dir = base_dir + "/gist1m"
# data_name = "gist"
# label = F"{tag}.gist1M"
# raw_file = F"output.{label}.raw.txt"
#
# subprocess.run(F':> {raw_file}', shell=True, check=True)
# L_min=L_lower
# L_max=L_upper
# command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
#           F"{L_min} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.q-1000.binary " \
#           F"{num_t} {L_max} {L_step} {X_low} {X_step} " \
#           F"| tee -a {raw_file}"
# subprocess.run(command, env=env_vars, shell=True, check=True)
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
# L_min=L_lower
# L_max=L_upper
# command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
#           F"{L_min} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.q-10000.binary " \
#           F"{num_t} {L_max} {L_step} {X_low} {X_step} " \
#           F"| tee -a {raw_file}"
# subprocess.run(command, env=env_vars, shell=True, check=True)
#
# rows_file = F"output.{label}.rows.txt"
# table_file = F"output.{label}.table.txt"
# selected_file = F"output.{label}.table.selected.txt"
# subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 9 12 13 15 1', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 2 0', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)
