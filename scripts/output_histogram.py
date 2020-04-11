#! python3
import sys

if len(sys.argv) != 4:
    print(f'{sys.argv[0]} <input_file> <output_file> <value_L>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    # num_repeat = int(sys.argv[3])
    # Get indices selected
    # selected_indices = int(sys.argv[4])
    # is_first_line = True
    # i_rp = 0
    value_L = int(sys.argv[3])
    columns_nums = []
    columns_titles = []
    # for i in range(num_repeat):
    #     columns.append([])
    for line in fin:
        line = line.strip()
        if line[0] in ['%', '#', '=', '+', 'F', 'N']:
        # if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
            is_first_line = True
            fout.write(line + '\n')
            i_rp = 0
            continue
        # columns = line.split()
        if line[0] == 'q':
            continue
        elif line[0] == 'i':
            columns_titles.append(line)
            columns_nums.append([])
            num_count = 0
            continue
        columns_nums[-1].append(line)
        # columns_nums[-1].append(float(line))

    # Write to the file
    num_col = len(columns_titles)
    for i in range(num_col):
        if i == 0:
            fout.write(F"{columns_titles[i]} Count")
            # fout.write(columns_titles[i])
        else:
            fout.write(F" {columns_titles[i]} Count")
            # fout.write(" " + columns_titles[i])
    fout.write("\n")

    # print(F"value_L: {value_L} num_col: {num_col} len(columns_nums): {len(columns_nums)} len(columns_nums[0]): {len(columns_nums[0])}")
    for i_raw in range(value_L):
        for i_col in range(num_col):
            if i_col == 0:
                fout.write(str(columns_nums[i_col][i_raw]))
            else:
                # print(F"i_col: {i_col} i_raw: {i_raw} len(columns_nums): {len(columns_nums)} len(columns_nums[{i_col}]): {len(columns_nums[i_col])}")
                fout.write(" " + str(columns_nums[i_col][i_raw]))
        fout.write("\n")
