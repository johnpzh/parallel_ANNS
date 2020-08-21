#! python3
import sys

if len(sys.argv) < 4:
    # print(f'{sys.argv[0]} <input_file> <output_file> <repeat> <index_list>')
    print(f'{sys.argv[0]} <input_file> <output_file> <index_list>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    # selected_indices = [0, 1, 2, 3]
    # Get indices selected
    # selected_indices = [int(i) for i in sys.argv[3:]]
    selected_indices = []
    for arg in sys.argv[3:]:
        if ":" not in arg:
            selected_indices.append(int(arg))
        else:
            # Support A:B
            mid = arg.find(":")
            start = int(arg[0:mid])
            bound = int(arg[mid + 1:len(arg)])
            for index in range(start, bound):
                selected_indices.append(index)


    is_first_line = True
    for line in fin:
        line = line.strip()
        if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
            is_first_line = True
            fout.write(line + '\n')
            continue

        # Get all columns
        columns = line.split()

        # Print column titles
        if is_first_line:
            is_first_line = False
            output = []
            for si in selected_indices:
                if 2 * si < len(columns):
                    output.append(columns[2 * si])
            fout.write('\t'.join(output) + '\n')

        # Print column values
        output = []
        for si in selected_indices:
            if 2 * si + 1 < len(columns):
                output.append(columns[2 * si + 1])
        fout.write('\t'.join(output) + '\n')
