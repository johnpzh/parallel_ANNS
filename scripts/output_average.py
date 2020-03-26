#! python3
import sys

if len(sys.argv) != 5:
    print(f'{sys.argv[0]} <input_file> <output_file> <repeat> <selected_index>')
    # print(f'{sys.argv[0]} <input_file> <output_file> <index_list>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    num_repeat = int(sys.argv[3])
    # Get indices selected
    selected_indices = int(sys.argv[4])
    is_first_line = True
    i_rp = 0
    columns = []
    for i in range(num_repeat):
        columns.append([])
    for line in fin:
        line = line.strip()
        if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
            is_first_line = True
            fout.write(line + '\n')
            i_rp = 0
            continue

        # Get all columns
        columns[i_rp] = line.split()
        i_rp += 1

        # Print column titles
        if is_first_line:
            is_first_line = False
            fout.write(columns[0][0] + " " + columns[0][2 * selected_indices] + '\n')

        # Print column values
        if i_rp == num_repeat:
            i_rp = 0
            tmp_list = [float(columns[e_i][2 * selected_indices + 1]) for e_i in range(num_repeat)]
            min_element = min(tmp_list)
            fout.write(str(columns[0][1]) + " " + str(min_element) + '\n')
