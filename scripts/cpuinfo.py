#! python3

with open("/proc/cpuinfo") as fin:
    print("processor:\tphysical id:\tcore id:")
    for line in fin:
        line = line.strip()
        # print(line)
        # continue
        # if line == "":
        #     continue
        columns = line.split()
        if len(columns) == 0:
            continue
        if columns[0] == "processor":
            print(columns[-1], end="")
        elif columns[0] == "physical":
            print(F"\t{columns[-1]}", end="")
        elif columns[0] == "core":
            print(F"\t{columns[-1]}")
