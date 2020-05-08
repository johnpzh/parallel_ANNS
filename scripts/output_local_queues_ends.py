#! python3
import sys

if len(sys.argv) != 4:
    print(f'{sys.argv[0]} <input_file> <output_file> <num_threads>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    num_threads = int(sys.argv[3])
    query_id = 0


    # Get indices selected
    for line in fin:
        line = line.strip()
        columns = line.split()
        if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
            # Print Titles
            fout.write(F"query:{columns[2]}iter:")
            for t_i in range(num_threads):
                fout.write(F" [{t_i}]")
            fout.write("\n")
            query_id += 1
            continue

        for c_i in range(num_threads + 1):
            if 0 != c_i:
                fout.write(" ")
            fout.write(columns[c_i * 2 + 1])
        fout.write("\n")
