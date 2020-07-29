#! python3
import sys

if len(sys.argv) != 7:
    print(f'Usage: {sys.argv[0]} <input_file1> <input_file2> <input_file3> <input_file4> <input_file5> <output_curve_file>')
    exit()
with open(sys.argv[1]) as fin_1, \
        open(sys.argv[2]) as fin_2, \
        open(sys.argv[3]) as fin_3, \
        open(sys.argv[4]) as fin_4, \
        open(sys.argv[5]) as fin_5, \
        open(sys.argv[6], 'w') as fout:
    # columns = []

    # is_first = False
    # Read accuracy and query-per-second and write
    # last_acc = ""
    for line_1, line_2, line_3, line_4, line_5 in zip(fin_1, fin_2, fin_3, fin_4, fin_5):
        line_1 = line_1.strip()
        line_2 = line_2.strip()
        line_3 = line_3.strip()
        line_4 = line_4.strip()
        line_5 = line_5.strip()
        fout.write(line_1 + "\t" + line_2 + "\t" + line_3 + "\t" + line_4 + "\t" + line_5 + "\n")
        # if is_first:
        #     is_first = False
        #     continue
        # line_acc = line_acc.strip()
        # line_qps = line_qps.strip()
        # if line_acc == last_acc:
        #     continue
        # if line_acc[0] in ['%', '#', '-', '=', '+', 'F', 'N']:
        #     is_first = True
        #     fout_curv.write(line_acc + "\n")
        # else:
        #     fout_curv.write(line_acc + "\t" + line_qps + "\n")
        # last_acc = line_acc
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
