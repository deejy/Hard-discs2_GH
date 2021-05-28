import os
from matplotlib import pyplot as plt
import numpy as np
import math
from object_file import Object_simulated
#OLD_algo
#number = line[4]
#obj_n_bad = line[7]
#obj_n_good = line[10]
#obj_dl_max = line[13]
#dx = line[15]
#dy = line[17]
#angle = line[19]

###PERMET DE LIRE LES STANDARD DEVIATION (proposed) DES OBJECTS

def read_content_obj_sd():
    """
    read from logger (OLD_algo) the sd for the normal distribution of each object
    """
    input_file = open("/home/alexandre/github/hard_discs2-master/test_alex/NEW_algorithme_movement.txt", "r")
    file_handler = input_file.readlines()
    list_parameters = []
    dico_information_obj = {"0":[],"1":[], "2":[], "3":[], "4":[], "5":[], "6":[], "7":[], "8":[], "9":[], "10":[],"11":[],"12":[],"13":[],"14":[]}
    for line in file_handler:
        if "rotate" in line:
            line = line.split(' ')
            print(line)
            dico_information_obj[str(line[4])].append(float(line[9][:-1])*90)
    input_file.close()
    return dico_information_obj


def draw_grap(dico):
    figure, axis = plt.subplots(5, 3)
    columns = 0
    line = 0
    for key, value in dico.items():
        axis[line, columns].plot(value)
        axis[line, columns].set_xlim([0, 3500])
        axis[line, columns].set_ylim([0, 7.5])

        columns += 1
        if columns > 2 :
            columns = 0
            line += 1
    plt.title("standard deviation")
    plt.show()




if __name__ == "__main__":

    NEW_dico_obj = read_content_obj_sd()
    print(NEW_dico_obj)
    draw_grap(NEW_dico_obj)
