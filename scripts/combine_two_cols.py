#! python3
import sys

if len(sys.argv) != 4:
    print(f'Usage: {sys.argv[0]} <input_query-per-second_file> <input_accuracy_file> <output_curve_file>')
    exit()
with open(sys.argv[1]) as fin_qps, \
        open(sys.argv[2]) as fin_acc, \
        open(sys.argv[3], 'w') as fout_curv:
    # columns = []

    is_first = False
    # Read accuracy and query-per-second and write
    last_acc = ""
    for line_acc, line_qps in zip(fin_acc, fin_qps):
        if is_first:
            is_first = False
            continue
        line_acc = line_acc.strip()
        line_qps = line_qps.strip()
        if line_acc == last_acc:
            continue
        if line_acc[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
            is_first = True
            fout_curv.write(line_acc + "\n")
        else:
            fout_curv.write(line_acc + " " + line_qps + "\n")
        last_acc = line_acc
    #
    #
    # # Read accuracy
    # for line in fin_acc:
    #     if is_first:
    #         is_first = False
    #         continue
    #     line = line.strip()
    #     if len(columns) > 0 and line == columns[-1][0]:
    #         continue
    #     columns.append([])
    #     columns[-1].append(line)
    #     if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
    #         is_first = True
    #
    # is_first = False
    # # Read latency
    # i_r = 0
    # for line in fin_qps:
    #     line = line.strip()
    #     if line[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
    #         is_first = True
    #         i_r += 1
    #         continue
    #     if is_first:
    #         is_first = False
    #         continue
    #     columns[i_r].append(line)
    #     i_r += 1
    #
    # # Write curve
    # for i_r in range(len(columns)):
    #     fout_curv.write(" ".join(columns[i_r]) + "\n")
