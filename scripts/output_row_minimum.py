#! python3
import sys

if len(sys.argv) != 5:
    print(f'{sys.argv[0]} <input_file> <output_file> <repeat> <min_col_index>')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    num_repeat = int(sys.argv[3])
    # Get indices selected
    selected_index = int(sys.argv[4])
    is_first_line = True

    rows = []
    for i in range(num_repeat):
        rows.append([])

    row_i = 0
    for line in fin:
        line = line.strip()
        if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
            is_first_line = True
            fout.write(line + '\n')
            continue
        if is_first_line:
            is_first_line = False
            fout.write(line + "\n")
            continue

        rows[row_i] = line.split()
        row_i += 1
        if row_i == num_repeat:
            row_i = 0
            # Select the minimum
            min_num = float(rows[0][selected_index])
            min_sub = 0
            for r_i in range(num_repeat - 1):
                sub = r_i + 1
                tmp_v = float(rows[sub][selected_index])
                if (tmp_v < min_num):
                    min_num = tmp_v
                    min_sub = sub
            # Output
            fout.write("\t".join(rows[min_sub]) + "\n")


    # i_rp = 0
    # columns = []
    # for i in range(num_repeat):
    #     columns.append([])
    # for line in fin:
    #     line = line.strip()
    #     if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
    #         is_first_line = True
    #         fout.write(line + '\n')
    #         i_rp = 0
    #         continue
    #
    #     # Get all columns
    #     columns[i_rp] = line.split()
    #     i_rp += 1
    #
    #     # Print column titles
    #     if is_first_line:
    #         is_first_line = False
    #         fout.write(columns[0][2 * selected_indices] + '\n')
    #         # fout.write(columns[0][0] + " " + columns[0][2 * selected_indices] + '\n')
    #
    #     # Print column values
    #     if i_rp == num_repeat:
    #         i_rp = 0
    #         tmp_list = [float(columns[e_i][2 * selected_indices + 1]) for e_i in range(num_repeat)]
    #         min_element = min(tmp_list)
    #         fout.write(str(min_element) + '\n')
    #         # fout.write(str(columns[0][1]) + " " + str(min_element) + '\n')
