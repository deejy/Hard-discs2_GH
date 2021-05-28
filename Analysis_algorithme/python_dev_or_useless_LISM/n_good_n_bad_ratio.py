import os
from matplotlib import pyplot as plt


def read_content(path,nrb_simulation):
    """
    read content from Algorithm LISM.
    @return list_ratio_final : 5 list containing the line this the global ratio of each saved simulation
    """
    list_ratio_final = []
    for y in range(1,nrb_simulation+1):
        input_file = open(path+"hard_discs2-master/test_alex/test_encomb_10/test_attract_"+str(y)+"/test2.log", "r")
        file_handler = input_file.readlines()
        list = []
        index_line =0
        for line in file_handler:
            index_line += 1
            if index_line == 9:
                index_line -= 18
                list.append(line)
        list_ratio_final.append(list)
    return list_ratio_final

def determine_mobility_ratio(all_list):
    """
    create different global ratio of mobility during all the simulation
    from the lists (from the function read_content).
    """
    list_all_ratio = []
    for list in all_list:
        list_ratio = []
        for line in list:
            line = line.split(",")
            line = line[0]
            line = line.split(' ')
            ratio = int(line[1])/ (int(line[3])+ int(line[1]))
            print(line)
            list_ratio.append(ratio)
        list_all_ratio.append(list_ratio)
    return list_all_ratio


def draw_grap(list_all_ratio, nrb_simulation):
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.set_xlabel('n step')
    ax.set_ylabel('move accepted / move accepted + refused')
    index = 0
    label_name = "simulation n° " + str(index)
    for list in list_all_ratio:
        index += 1
        if index < nrb_simulation +0.5:
            ax.plot(list, 'b', label=label_name)
        else:
            ax.plot(list, 'r', label=label_name)
        label_name = "simulation n° " + str(index)
    plt.gca().legend()
    plt.show()
    pass



nrb_simulation = 1
list_ratio_bf_analysis_new = read_content("/home/alexandre/github/",nrb_simulation)
list_ratio_bf_analysis_OLD = read_content("/home/alexandre/github/OLD_folder_LISM/",nrb_simulation)
list_list = (list_ratio_bf_analysis_new + list_ratio_bf_analysis_OLD)
list_ratio_af_analysis = determine_mobility_ratio(list_list)

draw_grap(list_ratio_af_analysis, nrb_simulation)
#(list_ratio_bf_analysis)
