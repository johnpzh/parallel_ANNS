#! python3
import sys

if len(sys.argv) != 3:
    print(f'{sys.argv[0]} <input_file> <output_file>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    is_entered = False
    for line in fin:
        line = line.strip()
        if line[0:4] == "----":
            is_entered = True
            continue
        if is_entered:
            fout.write(line + "\n")
            is_entered = False
        ######
