import argparse
import networkx as nx

def get_duplicate_vertex(v_id: int):
    s_a = v_id * 10
    s_b = v_id * 10 + 1
    return s_a, s_b

def get_mini_cut(args):
    input_file = args.input_txt
    output_file = args.output_txt
    G = nx.DiGraph()
    with open(input_file, "r") as fin, \
            open(output_file, "w") as fout:
        ## Read source
        source_id = int(fin.readline())
        print(f"source_id: {source_id}")

        ## Read targets
        targets = list()
        num_targets = int(fin.readline())
        print(f"num_targets: {num_targets}")
        for _ in range(num_targets):
            t_id = int(fin.readline())
            targets.append(t_id)

        ## Read edges
        num_edges = int(fin.readline())
        print(f"num_edges: {num_edges}")
        for _ in range(num_edges):
            si, ei = fin.readline().split()
            si = int(si)
            ei = int(ei)
            ## Add edge to the graph
            s_a, s_b = get_duplicate_vertex(si)
            e_a, e_b = get_duplicate_vertex(ei)
            G.add_edge(s_a, s_b, capacity=1.0)
            G.add_edge(s_b, e_a, capacity=1.0)
            G.add_edge(e_a, e_b, capacity=1.0)
            # G.add_edge(si, ei, capacity=1.0)

        ## Get mini cut
        s_a, s_b = get_duplicate_vertex(source_id)
        cutset = set()
        cnt_t = 0
        for t_i in targets:
            t_a, t_b = get_duplicate_vertex(t_i)
            cut_value, partition = nx.minimum_cut(G, s_b, t_a)
            # cut_value, partition = nx.minimum_cut(G, s_a, t_b)
            reachable, non_reachable = partition
            for u, nbrs in ((n, G[n]) for n in reachable):
                cutset.update((u, v) for v in nbrs if v in non_reachable)
            print(f"i: {cnt_t} t_i: {t_i} cut_value: {cut_value} len(cutset): {len(cutset)}")
            cnt_t += 1
        ## Print out
        print(f"len(cutset): {len(cutset)}")
        num_cut_edges = len(cutset)
        vertex_set = set()
        c_i = 0
        for cut in cutset:
            print(f"c_i: {c_i} {cut[0]} {cut[1]}")
            c_i += 1
            vertex_set.add(cut[0])
            vertex_set.add(cut[1])

        ## Save cut
        num_cut_vertices = len(vertex_set)
        fout.write(f"{num_cut_vertices}\n")
        for v_id in vertex_set:
            fout.write(f"{v_id}\n")
        fout.write(f"{num_cut_edges}\n")
        for s, e in cutset:
            fout.write(f"{s}\t{e}\n")


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_txt", help="input text file")
    parser.add_argument("output_txt", help="output text file containing cut vertex set")
    # parser.add_argument("output_fig", help="output figure file")
    # parser.add_argument("query_id", help="query id", type=int)
    args = parser.parse_args()
    return args


def main(args):
    get_mini_cut(args)


if __name__ == "__main__":
    args = get_args()
    main(args)