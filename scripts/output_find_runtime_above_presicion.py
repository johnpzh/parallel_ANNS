#! python3
import sys

def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

# if len(sys.argv) != 5:
#     print(f'{sys.argv[0]} <input_file> <output_file> <runtime_pos> <precision_pos>')
#     exit()
if len(sys.argv) < 6:
    print(f'{sys.argv[0]} <input_file> <output_file> <runtime_pos> <precision_pos> <precision_target> [<precision_target> ...]')
    exit()
with open(sys.argv[1]) as fin, \
        open(sys.argv[2], 'w') as fout:
    runtime_pos = int(sys.argv[3])
    precision_pos = int(sys.argv[4])

    # Get the input table
    table = []
    for line in fin:
        line = line.strip()
        cols = line.split()
        if not is_number(cols[0]):
            fout.write(line + "\n")
            continue
        table.append(cols)

    # bottoms = [0.90, 0.91, 0.92, 0.93,
    #            0.94, 0.95, 0.96, 0.97,
    #            0.98, 0.99,
    #            0.991, 0.992, 0.993, 0.994,
    #            0.995, 0.996, 0.997, 0.998,
    #            0.999]
    base_loc_bottoms = 5
    bottoms = [float(sys.argv[i]) for i in range(base_loc_bottoms, len(sys.argv))]
    for precs_bottom in bottoms:
        min_rt = sys.float_info.max
        min_id = -1

        for row_id in range(len(table)):
            row = table[row_id]
            # if not is_number(row[0]):
            #     fout.write("\t".join(row) + "\n")
            #     continue
            runtime = float(row[runtime_pos])
            precision = float(row[precision_pos])
            if precision >= precs_bottom and runtime < min_rt:
                min_rt = runtime
                min_id = row_id
        if min_id != -1:
            fout.write("\t".join(table[min_id]) + "\n")
        else:
            fout.write(f"Not found {precs_bottom}\n")

#
#
# ################
#     rows = []
#     for i in range(num_repeat):
#         rows.append([])
#
#     row_i = 0
#     for line in fin:
#         line = line.strip()
#         if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
#             is_first_line = True
#             fout.write(line + '\n')
#             continue
#         if is_first_line:
#             is_first_line = False
#             fout.write(line + "\n")
#             continue
#
#         rows[row_i] = line.split()
#         row_i += 1
#         if row_i == num_repeat:
#             row_i = 0
#             # Select the minimum
#             min_num = float(rows[0][selected_index])
#             min_sub = 0
#             for r_i in range(num_repeat - 1):
#                 sub = r_i + 1
#                 tmp_v = float(rows[sub][selected_index])
#                 if (tmp_v < min_num):
#                     min_num = tmp_v
#                     min_sub = sub
#             # Output
#             fout.write("\t".join(rows[min_sub]) + "\n")
