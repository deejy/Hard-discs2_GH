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
#PERMET DE LIRE LES ROTATIONS ACCEPTED


def read_content_rotation(old_or_new):
    """
    read from logger (OLD_algo) the obj_dl_max of each object
    """
    input_file = open("/home/alexandre/github/hard_discs2-master/test_alex/"+str(old_or_new)+"_algorithme_movement.txt", "r")
    file_handler = input_file.readlines()
    dico_information_angle_rotation = {"0":[],"1":[], "2":[], "3":[], "4":[], "5":[], "6":[], "7":[], "8":[], "9":[], "10":[],"11":[],"12":[],"13":[],"14":[]}

    for line in file_handler:
        if "after_25000" in line:
            if "accepted" in line:
                line = line.split(' ')
                dico_information_angle_rotation[str(line[4])].append(float(line[14]))
    input_file.close()
    return dico_information_angle_rotation


def draw_grap(dico):
    """
    take a dico containing 14 key and list as values. and create a plot for each
    """
    figure, axis = plt.subplots(5, 3)
    columns = 0
    line = 0
    for key, value in dico.items():
        axis[line, columns].plot(value)
        axis[line, columns].set_xlim([0, 50])
        axis[line, columns].set_ylim([0, 15])
        columns += 1
        if columns > 2 :
            columns = 0
            line += 1
    plt.show()

def create_histograms_from_list(data):
    # fixed bin siz fixed bin size

    plt.xlim([0, 360])
    plt.ylim([0, 50])
    plt.hist(data, 360)
    plt.title('distribution of rotation')
    plt.xlabel('angle')
    plt.ylabel('count')

    plt.show()


if __name__ == "__main__":

    dico_rotation_accepted = read_content_rotation("NEW")
    list = []
    for key, value in dico_rotation_accepted.items():
        for rot in value:
            rot *= 180/math.pi
            list.append(rot)
    print(dico_rotation_accepted)
    create_histograms_from_list(list)
    print("===========================================================================")
    #draw_grap(OLD_dico_obj_dl_max)
    #draw_grap(NEW_dico_obj_dl_max)
