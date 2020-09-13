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
    # is_first_line = True

    rows = []
    for i in range(num_repeat):
        rows.append([])

    row_i = 0
    for line in fin:
        line = line.strip()
        if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']\
                or not line[0].isnumeric():
            # is_first_line = True
            fout.write(line + '\n')
            row_i = 0
            continue
        # if is_first_line:
        #     is_first_line = False
        #     fout.write(line + "\n")
        #     continue

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
