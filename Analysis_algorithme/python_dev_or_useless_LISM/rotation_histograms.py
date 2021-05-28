import os
from matplotlib import pyplot as plt
import numpy as np
import math


def read_content_get_rotation(path,nrb_simulation):
    """
    read content from Algorithm LISM.
    @return list_containing_line : 5 list containing the line this the global ratio of each saved simulation
    """
    list = []
    for y in range(1,nrb_simulation+1):
        input_file = open(path+"hard_discs2-master/test_alex/emp.txt", "r")
        file_handler = input_file.readlines()
        list = []
        index_line = 0
        for line in file_handler:
            if "rotate" in line:
                line = line.split(' ')
                obj_number = int(line[4])
                value = float(line[-2])
                #value *= 180/math.pi
                #if value > 360 :
                #    value -= 360
                #elif value < 0:
                #    value += 360
                list.append(value)
    return list

def create_histograms_from_list(data):
    # fixed bin siz fixed bin size

    plt.xlim([0, 20])

    plt.hist(data, 1000)
    plt.title('distribution of rotation')
    plt.xlabel('angle')
    plt.ylabel('count')

    plt.show()

nrb_simulation = 1
list_new_rotation = read_content_get_rotation("/home/alexandre/github/",nrb_simulation)
list_OLD_rotation = read_content_get_rotation("/home/alexandre/github/OLD_folder_LISM/",nrb_simulation)

create_histograms_from_list(list_new_rotation)
#create_histograms_from_list(list_OLD_rotation)
