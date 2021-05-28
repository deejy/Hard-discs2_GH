import os
from matplotlib import pyplot as plt
import math

def read_content_get_rotation(path,nrb_simulation):
    """
    read content from Algorithm LISM.
    @return list_containing_line : 5 list containing the line this the global ratio of each saved simulation
    """
    dico_information_rotation = {}
    for i in range(0,14):
        dico_information_rotation[i]=[]
    list_containing_line = []
    for y in range(1,nrb_simulation+1):
        input_file = open(path+"hard_discs2-master/test_alex/emp.txt", "r")
        file_handler = input_file.readlines()
        list = []
        index_line =0
        for line in file_handler:
            if "rotate" in line:
                line = line.split(' ')
                obj_number = int(line[4])
                value = float(line[-2])
                dico_information_rotation[obj_number].append(value)
    return dico_information_rotation

def read_content_get_mobility(path,nrb_simulation):
    """
    read content from Algorithm LISM.
    @return list_containing_line : 5 list containing the line this the global ratio of each saved simulation
    """
    dico_information_mobility = {}
    for i in range(0,14):
        dico_information_mobility[i]=[]
    list_containing_line = []
    for y in range(1,nrb_simulation+1):
        input_file = open(path+"hard_discs2-master/test_alex/emp.txt", "r")
        file_handler = input_file.readlines()
        list = []
        index_line =0
        for line in file_handler:
            if "rotate" in line:
                line = line.split(' ')
                obj_number = int(line[4])
                mobility = float(line[-7][:-1])
                dico_information_mobility[obj_number].append(mobility)
    return dico_information_mobility


def draw_graph(dico_information_rotation, dico_information_mobility):

    index = 1
    #label_name = "simulation n° " + str(index)
    for key, values in dico_information_rotation.items():
        index += 1
        fig = plt.figure()
        ax = fig.add_subplot(1, 1, 1)
        ax.set_xlabel('time')
        ax.set_ylabel('angle of rotation radian')
        ax.plot(values, antialiased=False, label=key)
        ax.plot(dico_information_mobility[key], antialiased=False, label=key)
        #label_name = "simulation n° " + str(index)
        plt.axhline(y=0, color='r', linestyle='-')
        plt.gca().legend()
        plt.show()

nrb_simulation = 1

dico_new_rotation = read_content_get_rotation("/home/alexandre/github/",nrb_simulation)
dico_OLD_rotation = read_content_get_rotation("/home/alexandre/github/OLD_folder_LISM/",nrb_simulation)

new_mobi = read_content_get_mobility("/home/alexandre/github/",nrb_simulation)
old_mobi = read_content_get_mobility("/home/alexandre/github/OLD_folder_LISM/",nrb_simulation)


draw_graph(dico_new_rotation, new_mobi)
draw_graph(dico_OLD_rotation, old_mobi)
