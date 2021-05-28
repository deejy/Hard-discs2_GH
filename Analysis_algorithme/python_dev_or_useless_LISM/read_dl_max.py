import os
from matplotlib import pyplot as plt


def read_content(path,nrb_simulation):
    """
    read content from Algorithm LISM.
    @return list_containing_line : 5 list containing the line this the global ratio of each saved simulation
    """
    list_containing_line = []
    for y in range(1,nrb_simulation+1):
        input_file = open(path+"hard_discs2-master/test_alex/test_encomb_30/test_attract_"+str(y)+"/test2.log", "r")
        file_handler = input_file.readlines()
        list = []
        index_line =0
        for line in file_handler:
            index_line += 1
            if index_line == 9:
                index_line -= 18
                list.append(line)
        list_containing_line.append(list)
        input_file.close()
    return list_containing_line

def read_dist_max(list_containing_line):
    final_list = []
    for list in list_containing_line:
        list_DIST_max = []
        for line in list:
            line = line.split('=')
            line = line[1][:-1]
            list_DIST_max.append(float(line))
        final_list.append(list_DIST_max)
    return final_list


def draw_graph(list_all_ratio):
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.set_xlabel('n_step')
    ax.set_ylabel('Dist_max')
    index = 1
    label_name = "simulation n° " + str(index)
    for list in list_all_ratio:
        index += 1
        ax.plot(list, antialiased=False, label=label_name)
        label_name = "simulation n° " + str(index)
    plt.gca().legend()
    plt.show()

nrb_simulation = 1
list_ratio_bf_analysis_new = read_content("/home/alexandre/github/",nrb_simulation)
list_ratio_bf_analysis_OLD = read_content("/home/alexandre/github/OLD_folder_LISM/",nrb_simulation)
draw_graph(read_dist_max(list_ratio_bf_analysis_new+list_ratio_bf_analysis_OLD))
