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
#PERMET DE LIRE LES OBJ_DL_MAX


def read_content_obj_dl_max(old_or_new):
    """
    read from logger (OLD_algo) the obj_dl_max of each object
    """
    input_file = open("/home/alexandre/github/hard_discs2-master/test_alex/"+str(old_or_new)+"_algorithme_movement.txt", "r")
    file_handler = input_file.readlines()
    list_parameters = []
    dico_information_obj_dl_max = {"0":[],"1":[], "2":[], "3":[], "4":[], "5":[], "6":[], "7":[], "8":[], "9":[], "10":[],"11":[],"12":[],"13":[],"14":[]}
    for line in file_handler:
        if "dl_max" in line:
            line = line.split(' ')
            print(line)
            dico_information_obj_dl_max[str(line[4])].append(float(line[13][:-1]))
    input_file.close()
    return dico_information_obj_dl_max


def draw_grap(dico):
    figure, axis = plt.subplots(5, 3)
    columns = 0
    line = 0
    for key, value in dico.items():
        axis[line, columns].plot(value)
        axis[line, columns].set_xlim([0, 1500])
        axis[line, columns].set_ylim([0, 15])
        columns += 1
        if columns > 2 :
            columns = 0
            line += 1
    plt.show()






if __name__ == "__main__":
    OLD_dico_obj_dl_max = read_content_obj_dl_max("OLD")
    NEW_dico_obj_dl_max = read_content_obj_dl_max("NEW")
    print(OLD_dico_obj_dl_max)
    print(NEW_dico_obj_dl_max)
    print("===========================================================================")
    draw_grap(OLD_dico_obj_dl_max)
    draw_grap(NEW_dico_obj_dl_max)
