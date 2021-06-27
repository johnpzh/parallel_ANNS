import sys
import argparse
import igraph as ig


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_edgelist", help="input text file of the edgelist")
    parser.add_argument("output_figure", help="output pdf file of the graph layout")
    args = parser.parse_args()

    return args


def get_no_suffix_name(filename: str):
    dot_pos = filename.rfind(".")
    if dot_pos != -1:
        return filename[:dot_pos]
    else:
        return filename


def get_K_results(input_file: str, value_k: int):
    results = list()
    with open(input_file, "r") as fin:
        for line in fin:
            results.append(int(line.strip()))
        assert value_k == len(results), f"Error: result file contains {len(results)} results, but K = {value_k}."
    return results


def plot(input_file, output_file):
    g = ig.Graph.Read_Ncol(input_file)
    num_v = g.vcount()
    num_e = g.ecount()
    print(f"num_v: {num_v} num_e: {num_e}")

    K = 100
    no_suffix_name = get_no_suffix_name(input_file)
    k_filename = f"{no_suffix_name}_K{K}.txt"
    k_results = get_K_results(k_filename, K)
    # #### test
    # print(k_results)
    # #### end test


    # layout = g.layout("random")
    # layout = g.layout("circle")
    # layout = g.layout("large")
    layout = g.layout("tree")
    visual_style = dict()
    visual_style["vertex_size"] = 7
    visual_style["edge_arrow_size"] = 0.4
    # visual_style["edge_arrow_width"] = 1
    visual_style["layout"] = layout
    g.vs["color"] = "white"
    # g.vs["color"] = "blue"
    # g.vs["label"] = [str(v_i) for v_i in range(num_v)]

    # Paint K final results red
    for k_i in k_results:
        g.vs[k_i]["color"] = "red"
    ig.plot(g, target=output_file, bbox=(800, 800), **visual_style)


def main():
    args = get_args()
    input_edgelist = args.input_edgelist
    output_figure = args.output_figure
    print(f"input: {input_edgelist} output: {output_figure}")
    plot(input_edgelist, output_figure)


if __name__ == "__main__":
    main()
