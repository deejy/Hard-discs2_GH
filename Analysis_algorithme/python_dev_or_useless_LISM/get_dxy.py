import os
from matplotlib import pyplot as plt


def read_content_get_coord(path,nrb_simulation):
    """
    read content from Algorithm LISM.
    @return list_containing_line : 5 list containing the line this the global ratio of each saved simulation
    """
    dico_information_xy = {"dx":[], "dy":[]}
    list_containing_line = []
    input_file = open(path+"hard_discs2-master/test_alex/emp.txt", "r")
    file_handler = input_file.readlines()
    list = []
    index_line =0
    for line in file_handler:
        if "move" in line:
            line = line.split(' ')
            obj_number = int(line[4])
            value_x = float(line[-8])
            dico_information_xy["dx"].append(value_x)
            value_y = float(line[-5][:-1])
            dico_information_xy["dy"].append(value_y)
    return dico_information_xy


def draw_graph(dico_information_xy):
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.set_xlabel('time')
    ax.set_ylabel('translation factor')
    index = 1
    #label_name = "simulation n° " + str(index)
    for key, values in dico_information_xy.items():
        index += 1
        ax.plot(values, antialiased=False, label=key)
        #label_name = "simulation n° " + str(index)
    plt.gca().legend()
    plt.show()

def create_histograms_from_list(dico_information_xy):
    # fixed bin siz fixed bin size
    for key, data in dico_information_xy.items():
        plt.xlim([-1, 1])
        plt.hist(data, 1000)
        plt.title('distribution of rotation')
        plt.xlabel(key)
        plt.ylabel('count')

        plt.show()


nrb_simulation = 1
dico_new_xy = read_content_get_coord("/home/alexandre/github/",nrb_simulation)
dico_OLD_xy = read_content_get_coord("/home/alexandre/github/OLD_folder_LISM/",nrb_simulation)
draw_graph(dico_new_xy)
draw_graph(dico_OLD_xy)
#create_histograms_from_list(dico_OLD_xy)
#create_histograms_from_list(dico_new_xy)
#dico_OLD_rotation = read_content_get_coord("/home/alexandre/github/OLD_folder_LISM/",nrb_simulation)
#list_ratio_bf_analysis_OLD = read_content("/home/alexandre/github/OLD_folder_LISM/",nrb_simulation)
