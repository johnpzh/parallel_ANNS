#! python3
import sys

if len(sys.argv) != 3:
    print(f'{sys.argv[0]} <input_file> <output_file>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    # num_queries = int(sys.argv[3])
    # query_id = 0

    # double_columns = []
    print_columns = []
    graph_index = 0
    count_index = 1 # This is for the group size
    # count_index = 3 # This is for the group size
    for line in fin:
        line = line.strip()
        # columns = line.split()
        if line[0] in ['%', '#', '-', '=', '+', 'F', 'N', 'Q']:
            if count_index == graph_index:
                graph_index = 0
            if 0 == graph_index:
                print_columns.append([])
            graph_index += 1
            # continue
        print_columns[-1].append(line)

    for row in range(len(print_columns[0])):
        for col in range(len(print_columns)):
            fout.write(F"{print_columns[col][row]}\t")
        fout.write("\n")
