#! python3
import sys

if len(sys.argv) != 3:
    print(f'{sys.argv[0]} <input_file> <output_file>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    # num_queries = int(sys.argv[3])
    query_id = 0

    double_columns = []
    # Get indices selected
    for line in fin:
        line = line.strip()
        columns = line.split()
        if line[0] in ['%', '#', '-', '=', '+', 'F', 'N', 'Q']:
            # Print Titles
            double_columns.append([columns[0]])
            double_columns.append([columns[1]])
            continue
        double_columns[-2].append(columns[0])
        double_columns[-1].append(columns[1])

    num_queries = len(double_columns) // 2
    max_row = 0
    for q_i in range(num_queries):
        if max_row < len(double_columns[q_i * 2]):
            max_row = len(double_columns[q_i * 2])

    for row in range(max_row):
        for c_i in range(len(double_columns)):
            if row >= len(double_columns[c_i]):
                fout.write(" \t")
            else:
                fout.write(double_columns[c_i][row] + "\t")
        fout.write("\n")