#! python3
import sys

if len(sys.argv) < 4:
    print(f'{sys.argv[0]} <input_file> <output_file> <attributes_list>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    # Get indices selected
    attributes = []
    attributes.extend(sys.argv[3:])
    tmp_atts = list(attributes)
    tmp_atts.append('count_distance_computation:')

    for line in fin:
        line = line.strip()
        if line[0] in ['%', '#', '-', '=', '+']:
            is_first_line = True
            fout.write(line + '\n')
            continue

        # Print column titles
        if is_first_line:
            is_first_line = False
            fout.write(' '.join(tmp_atts) + '\n')

        # Get all columns
        columns = line.split()
        if columns[0] not in ['Found:', 'Not']:
            continue

        values = []
        for c_i in range(len(columns)):
            if columns[c_i] not in attributes:
                continue
            values.append(columns[c_i + 1])
        fout.write(' '.join(values) + '\n')


    ########
    #
    # is_first_line = True
    # for line in fin:
    #     line = line.strip()
    #     if line[0] in ['%', '#', '-', '=', '+']:
    #         is_first_line = True
    #         continue
    #
    #     # Get all columns
    #     columns = line.split()
    #
    #     # Print column titles
    #     if is_first_line:
    #         is_first_line = False
    #         output = []
    #         for si in selected_indices:
    #             output.append(columns[2 * si])
    #         fout.write(' '.join(output) + '\n')
    #
    #     # Print column values
    #     output = []
    #     for si in selected_indices:
    #         output.append(columns[2 * si + 1])
    #     fout.write(' '.join(output) + '\n')
