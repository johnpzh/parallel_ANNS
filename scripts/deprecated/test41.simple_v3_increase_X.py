#! python3
import os
import sys
import subprocess

if len(sys.argv) != 7:
    print(f"{sys.argv[0]} <data_dir> <tag> <num_t> <L_low> <L_up> <X_low>")
    # print(f"{sys.argv[0]} <data_dir> <tag>")
    exit()

data_dir = sys.argv[1]
tag = sys.argv[2]
num_t = int(sys.argv[3])
L_lower = int(sys.argv[4])
L_upper = int(sys.argv[5])
X_lower = int(sys.argv[6])
# X_upper = int(sys.argv[7])

env_vars = os.environ
# env_vars["KMP_AFFINITY"] = "verbose,granularity=fine,compact,1,0"
env_vars["KMP_AFFINITY"] = "granularity=fine,compact,1,0"
bin="./profile_para_single_query_search_simple_v3"
data_dir = data_dir + "/sift1m"
data_name = "sift"
label = tag
raw_file = F"output.{label}.raw.txt"
table_file = F"output.{label}.table.txt"

subprocess.run(F':> {raw_file}', shell=True, check=True)
subprocess.run(F':> {table_file}', shell=True, check=True)

for L in range(L_lower, L_upper):
    min_runtime = sys.float_info.max
    last_runtime = sys.float_info.max
    count_incr = 0
    count_larger = 0
    for X in range(X_lower, L + 5):
        raw_tmp_file = F"output.{label}.tmp.raw.txt"
        rows_tmp_file = F"output.{label}.tmp.rows.txt"
        table_tmp_file = F"output.{label}.tmp.table.txt"
        subprocess.run(F':> {raw_tmp_file}', shell=True, check=True)
        command = F"{bin} {data_dir}/{data_name}_base.fvecs {data_dir}/{data_name}_query.fvecs {data_dir}/{data_name}.nsg " \
                  F"{L} 100 output.ivecs {data_dir}/{data_name}.true-100_NN.q-10000.binary {num_t} {L} {X} " \
                  F"| tee -a {raw_tmp_file}"
        subprocess.run(command, env=env_vars, shell=True, check=True)

        subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_tmp_file} {rows_tmp_file} 2 3 10 12 13 15 1', shell=True, check=True)
        subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_tmp_file} {table_tmp_file} 2 0', shell=True, check=True)
        subprocess.run(F'cat {table_file} {table_tmp_file} > tmp.txt && mv tmp.txt {table_file}', shell=True, check=True)
        subprocess.run(F'cat {raw_file} {raw_tmp_file} > tmp.txt && mv tmp.txt {raw_file}', shell=True, check=True)

        with open(table_tmp_file) as fin:
            # for line in fin:
            line = fin.readline()
            line = line.strip()
            cols = line.split()
            runtime = float(cols[0])  # Get the last runtime
            if runtime < last_runtime:
                count_incr = 0
            else:
                count_incr += 1
                if count_incr == 8:
                    # Stop increasing X, starting the next L
                    break
            last_runtime = runtime
            if runtime < min_runtime:
                min_runtime = runtime
                count_larger = 0
            else:
                count_larger += 1
                if count_larger == 16:
                    # Stop increasing X, starting the next L
                    break

selected_file = F"output.{label}.table.selected.txt"
subprocess.run(F'python3 ../scripts/output_find_runtime_above_presicion.py {table_file} {selected_file} 0 2', shell=True, check=True)

# rows_file = F"output.{label}.rows.txt"
# table_file = F"output.{label}.table.txt"
# subprocess.run(F'python3 ../scripts/output_rows_to_table.py {raw_file} {rows_file} 2 3 10 12 13 15 1', shell=True, check=True)
# subprocess.run(F'python3 ../scripts/output_row_minimum.py {rows_file} {table_file} 2 0', shell=True, check=True)
