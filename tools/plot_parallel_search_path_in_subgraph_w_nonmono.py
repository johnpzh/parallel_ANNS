import argparse
import matplotlib.pyplot as plt
from matplotlib.patches import ConnectionPatch
from matplotlib.ticker import MultipleLocator
import matplotlib.colors as mcolors



def get_color(c_id: int):
    colors = mcolors.CSS4_COLORS
    names = list(colors)
    num_colors = len(names)
    stride = 2
    base = 14
    index = (c_id * stride + base) % num_colors

    return names[index]


# # color count: [blue, red, grey]
# def add_to_step_color(
#         step_color_count: dict,
#         v_step: int,
#         v_color: str):
#     if v_color not in ["tab:blue", "tab:red", "tab:grey"]:
#         raise ValueError("v_color should be in tab:blue, and tab:red.")
#
#     if v_step not in step_color_count:
#         # color_count: [blue, red]
#         step_color_count[v_step] = [0, 0, 0]
#
#     if v_color == "tab:blue":
#         step_color_count[v_step][0] += 1
#     elif v_color == "tab:red":
#         step_color_count[v_step][1] += 1
#     elif v_color == "tab:grey":
#         step_color_count[v_step][2] += 1


def add_one_to_step_count(
        step_count: dict,
        v_step: int):
    if v_step not in step_count:
        # color_count: [blue, red]
        step_count[v_step] = 0
    step_count[v_step] += 1


def plot(
        input_file: str,
        output_file: str,
        data_tag: str,
        query_id: int):
    fig, ax = plt.subplots(1, 1)
    # X ticks
    ax.xaxis.set_major_locator(MultipleLocator(1))
    ax.xaxis.set_major_formatter("{x:.0f}")

    # Axes labels
    ax.set_xlabel("Steps")
    ax.set_ylabel("Distance")
    # ax.set_title("A query of SIFT1M. \n"
    #              "All smallest-step paths from the source to 100 nearest neighbors.")

    # Size
    # fig.set_size_inches(4, 3)
    fig.set_dpi(200)

    with open(input_file, "r") as fin:
        # Read kNN
        K = int(fin.readline())
        print(f"K: {K}")
        knn = list()
        max_nn_dist = 0.0 # the maximum distance of all nearest neighbors
        for k_i in range(K):
            nn_id, nn_dist = fin.readline().split()
            nn_id = int(nn_id)
            nn_dist = float(nn_dist)
            knn.append(nn_id)
            if nn_dist > max_nn_dist:
                max_nn_dist = nn_dist
        found_knn = set()
        # # Add a horizontal line indicating the longest distance of nearest neighbors
        # ax.axhline(max_nn_dist, 0, max_step, color="tab:red", linestyle="dashed")

        steps_max_dist = dict() # every step's maximum distance
        step_count = dict() # how many points in each step
        max_step = 0 # the maximum steps of points
        max_dist = 0.0 # the maximum distance of points
        min_dist = float("inf") # the minimum distance of points
        # Read united_set
        # united_set = set()
        # united_x = list()
        # united_y = list()
        # num_in_united_set = int(fin.readline())
        # print(f"num_in_united_set: {num_in_united_set}")
        # for v_i in range(num_in_united_set):
        #     v_id, v_step, v_dist = fin.readline().split()
        #     v_id = int(v_id)
        #     v_step = int(v_step)
        #     v_dist = float(v_dist)
        #     united_set.add(v_id)
        #     max_step = max(v_step, max_step)
        #     max_dist = max(v_dist, max_dist)
        #     min_dist = min(v_dist, min_dist)
        #     if v_step not in steps_max_dist:
        #         steps_max_dist[v_step] = 0.0
        #     if steps_max_dist[v_step] < v_dist:
        #         steps_max_dist[v_step] = v_dist
        #     add_one_to_step_count(step_count, v_step)
        #
        #     united_x.append(v_step)
        #     united_y.append(v_dist)
        #     if v_id in knn:
        #         found_knn.add(v_id)
        # ax.scatter(united_x, united_y, c="whitesmoke", label="united", alpha=0.6)

        # Read num_threads
        num_threads = int(fin.readline())
        print(f"num_threads: {num_threads}")
        for th_id in range(num_threads):
            xs = list()
            ys = list()
            num_p = int(fin.readline())
            print(f"th_id: {th_id} num_points: {num_p}")
            # Read points
            for p_i in range(num_p):
                v_id, v_step, v_dist = fin.readline().split()
                v_id = int(v_id)
                v_step = int(v_step)
                v_dist = float(v_dist)
                # if v_id in united_set:
                #     continue
                max_step = max(v_step, max_step)
                max_dist = max(v_dist, max_dist)
                min_dist = min(v_dist, min_dist)
                if v_step not in steps_max_dist:
                    steps_max_dist[v_step] = 0.0
                if steps_max_dist[v_step] < v_dist:
                    steps_max_dist[v_step] = v_dist
                add_one_to_step_count(step_count, v_step)

                xs.append(v_step)
                ys.append(v_dist)
                if v_id in knn:
                    found_knn.add(v_id)
            ax.scatter(xs, ys, c=get_color(th_id), label=f"T{th_id}", alpha=0.6)

        # Y axis height
        ax.set_ylim(
            bottom=min_dist * 0.7,
            top=max_dist + (max_dist - min_dist) * 0.17)

        # Add a horizontal line indicating the longest distance of nearest neighbors
        ax.axhline(max_nn_dist, 0, max_step, color="tab:red", linestyle="dashed")

        # # Add text of color counts
        # for step in range(max_step + 1):
        #     if step not in step_count:
        #         continue
        #     x = step
        #     y = steps_max_dist[step] + (max_dist - min_dist) * 0.04
        #     cnt = step_count[step]
        #     ax.text(x, y, f"cnt:{cnt}", fontsize="xx-small", ha="center")
        #     print(f"Step: {step} Cnt.: {cnt}")

        # Read arrows
        # expanded_count_in_every_step = [set()] * (max_step + 1)
        expanded_count_in_every_step = list()
        for _ in range(max_step + 1):
            expanded_count_in_every_step.append(set())
        num_arr = int(fin.readline())
        print(f"num_arr: {num_arr}")
        for a_i in range(num_arr):
            sx, sy, ex, ey = fin.readline().split()

            # #### test
            # if a_i > 600:
            #     continue
            # #### end test

            sx = int(sx)
            sy = float(sy)
            ex = int(ex)
            ey = float(ey)
            # arrows.append([sx, sy, ex, ey])
            # color = "tab:green" if sx < ex else "tab:orange"
            if sx >= ex:
                color = "tab:orange"
            elif sy > ey:
                color = "tab:green"
            else:
                color = "tab:pink"
            ax.add_artist(ConnectionPatch((sx, sy), (ex, ey), "data", "data", arrowstyle="->", alpha=0.2, color=color))

            if sy not in expanded_count_in_every_step[sx]:
                expanded_count_in_every_step[sx].add(sy)
                # ##### test
                # print(f"len(expanded_count_in_every_step[{sx}]): {len(expanded_count_in_every_step[sx])}")
                # #### end test

        # Add text of color counts
        for step in range(max_step + 1):
            if step not in step_count:
                continue
            x = step
            y = steps_max_dist[step] + (max_dist - min_dist) * 0.04
            cnt = step_count[step]
            exp = len(expanded_count_in_every_step[step])
            ax.text(x, y, f"cnt:{cnt}\nexp:{exp}", fontsize="xx-small", ha="center")
            print(f"Step: {step} Cnt.: {cnt}")

        # Read count of filtered out
        cnt_filtered_out = int(fin.readline())
        print(f"cnt_filtered_out: {cnt_filtered_out}")

        # Figure title
        max_exp = 0
        for st in expanded_count_in_every_step:
            max_exp = max(max_exp, len(st))
        num_found = len(found_knn)
        ax.set_title(f"Query #{query_id} of {data_tag}, {num_threads} threads. Max_Expd: {max_exp}.\n"
                     f"Parallel search from the source to {num_found}(/{K}) nearest neighbors.")
        # # Legend
        # ax.legend(loc="lower left")
        # Save figure
        print("Saving...")
        fig.savefig(output_file)


# ####################################################
# fig, ax = plt.subplots(1, 1)
# vert_steps = [0, 1, 1, 2, 2, 2, 3, 3]
# vert_dists = [7.7, 6.4, 8.1, 5.5, 4.7, 4.4, 3.7, 3.1]
# ax.scatter(vert_steps, vert_dists, c="tab:blue")
#
# # X ticks
# ax.xaxis.set_major_locator(MultipleLocator(1))
# ax.xaxis.set_major_formatter("{x:.0f}")
#
# points = list(zip(vert_steps, vert_dists))
#
# # Arrow
# arr = ConnectionPatch(points[0], points[1], "data", "data", arrowstyle="->")
#
# ax.add_artist(arr)
# fig.savefig('output.matplotlib.png')
# # plt.savefig('output.matplotlib.png')

# #########################################
# fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(6, 3))
#
# # Draw a simple arrow between two points in axes coordinates
# # within a single axes.
# xyA = (0.2, 0.2)
# xyB = (0.8, 0.8)
# coordsA = "data"
# coordsB = "data"
# con = ConnectionPatch(xyA, xyB, coordsA, coordsB,
#                       arrowstyle="-|>", shrinkA=5, shrinkB=5,
#                       mutation_scale=20, fc="w")
# ax1.plot([xyA[0], xyB[0]], [xyA[1], xyB[1]], "o")
# ax1.add_artist(con)
#
# fig.savefig('output.matplotlib.png')
# # plt.savefig('output.matplotlib.png')

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_txt", help="input text file")
    parser.add_argument("output_fig", help="output figure file")
    # parser.add_argument("num_threads", help="number of threads", type=int)
    parser.add_argument("data_tag", help="dataset name in the figure title")
    parser.add_argument("query_id", help="query id", type=int)
    args = parser.parse_args()
    return args


def main(args):
    input_file = args.input_txt
    output_file = args.output_fig
    # num_t = args.num_threads
    data_tag = args.data_tag
    query_id = args.query_id
    plot(input_file, output_file, data_tag, query_id)


if __name__ == "__main__":
    args = get_args()
    main(args)