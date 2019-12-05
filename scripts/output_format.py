#! python3
import sys

if len(sys.argv) < 4:
    print(f'{sys.argv[0]} <input_file> <output_file> <index_list>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    # selected_indices = [0, 1, 2, 3]
    # Get indices selected
    selected_indices = []
    selected_indices.extend(int(i) for i in sys.argv[3:])
    is_first_line = True
    for line in fin:
        line = line.strip()
        if line[0] in ['%', '#', '-', '=', '+']:
            is_first_line = True
            continue

        # Get all columns
        columns = line.split()

        # Print column titles
        if is_first_line:
            is_first_line = False
            output = []
            for si in selected_indices:
                output.append(columns[2 * si])
            fout.write(' '.join(output) + '\n')

        # Print column values
        output = []
        for si in selected_indices:
            output.append(columns[2 * si + 1])
        fout.write(' '.join(output) + '\n')
