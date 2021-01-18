import os
import sys
import subprocess

app_label = "Top_M"
for data in ["sift1m", "gist1m", "deep10m", "sift100m", "deep100m"]:
    for num_t in [2, 4, 8]:
        for P_target in ["0.900", "0.990", "0.995", "0.997", "0.999"]:
            tag = F"{app_label}_T{num_t}_P{P_target}"
            label = F"{data}.{tag}"
            rows_file = F"output.{label}.rows.txt"
            table_file = F"output.{label}.table.txt"
            subprocess.run(F"python3 ../scripts/output_format.py {rows_file} {table_file} 0:12", shell=True, check=True)

        output_files = F"output.{data}.{app_label}_T{num_t}_P0.9"
        table_file = F"output.{data}.{app_label}_T{num_t}_collected.table.txt"
        subprocess.run(F"cat {output_files}*.table.txt > {table_file}", shell=True, check=True)

app_label = "Scale_M"
for data in ["sift1m", "gist1m", "deep10m", "sift100m", "deep100m"]:
    for num_t in [2, 4, 8]:
        for P_target in ["0.900", "0.990", "0.995", "0.997", "0.999"]:
            tag = F"{app_label}_T{num_t}_P{P_target}"
            label = F"{data}.{tag}"
            rows_file = F"output.{label}.rows.txt"
            table_file = F"output.{label}.table.txt"
            subprocess.run(F"python3 ../scripts/output_format.py {rows_file} {table_file} 0:12", shell=True, check=True)

        output_files = F"output.{data}.{app_label}_T{num_t}_P0.9"
        table_file = F"output.{data}.{app_label}_T{num_t}_collected.table.txt"
        subprocess.run(F"cat {output_files}*.table.txt > {table_file}", shell=True, check=True)