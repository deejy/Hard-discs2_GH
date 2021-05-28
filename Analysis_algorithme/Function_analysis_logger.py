import os
from matplotlib import pyplot as plt
import numpy as np
import math



def read_content(path):
    """
    Read from logger and return it like a list
    Parameters
        path : the absolut path to get the logger file create by hard-disc software
    Return
        list : containing all the text from the input file
    """
    input_file = open(path, "r")
    file_handler = input_file.readlines()
    input_file.close()
    list = []

    for line in file_handler:
        line = line.split('\n')
        without_empty_strings = []
        for string in line:
            if (string != ""):
                without_empty_strings.append(string)
        line = without_empty_strings
        line = "".join(line)
        list.append(line)
    return list

def divided_in_dico(list):
    """
    That function will get a list and sort line in a dico by obj_number
    Parameters
        list : containing all the text from the input file
    Return
        dico_information_by_number : DICTIONNARY containing all the information from the input file by sorted by object number
    """
    dico_information_by_number = {"0":[],"1":[], "2":[], "3":[], "4":[], "5":[], "6":[], "7":[], "8":[], "9":[], "10":[],"11":[],"12":[],"13":[]}
    del list[0]
    for line in list:
        line_splited = line.split(' ,o')
        line_splited = line_splited[0].split(' ')
        without_empty_strings = []
        for string in line_splited:
            if (string != ""):
                without_empty_strings.append(string)
        line_splited = without_empty_strings
        #print(line_splited)
        dico_information_by_number[str(line_splited[-1])].append(line)
    return dico_information_by_number
#===============================================================================

def get_obj_dl_max(dico, n_step_start, n_step_end, accepted_or_refuse, nbr_move):
    """
    This function take a dico from divided_in_dico() and will create a dico with the dl_max of each object
    Parameters
        dico : DICTIONNARY containing all the information from the input file by sorted by object number
        n_step_start (filter): the function didn't get the information if the n_step in the logger is lower than n_step_start
        n_step_end(filter) : the function didn't get the information if the n_step in the logger is higher than n_step_end
        accepted_or_refuse (filter) : this filter is usefull to get only one type of move (to use it write "accepted" or "refused")
        nbr_move (filter) : the function will get the information, only in the case of n_good + n_bad of the selected object is higher than nbr_move
    Return
        dico_information_by_number : DICTIONNARY containing dl_max of each object sorted by object
    """
    dico_information_by_number = {"0":[],"1":[], "2":[], "3":[], "4":[], "5":[], "6":[], "7":[], "8":[], "9":[], "10":[],"11":[],"12":[],"13":[]}
    for key, value in dico.items():
        for line in value:
            if accepted_or_refuse in line:
                line = line.split(' ')
                if int(line[-2]) >= n_step_start:
                    if int(line[-2]) <= n_step_end:
                        n_good = int(line[10][:-1])
                        n_bad = int(line[7][:-1])
                        nbr = n_good + n_bad
                        if nbr > nbr_move:
                            dico_information_by_number[key].append(float(line[13][:-1]))
                    else:
                        pass
                else:
                    pass
    return dico_information_by_number

def get_rotation(dico, n_step_start, n_step_end, accepted_or_refuse, nbr_move):
    """
    This function take a dico from divided_in_dico() and will create a dico with the angle of rotation (in degrees) of each object
    Parameters
        dico : DICTIONNARY containing all the information from the input file by sorted by object number
        n_step_start (filter): the function didn't get the information if the n_step in the logger is lower than n_step_start
        n_step_end(filter) : the function didn't get the information if the n_step in the logger is higher than n_step_end
        accepted_or_refuse (filter) : this filter is usefull to get only one type of move (to use it write "accepted" or "refused")
        nbr_move (filter) : the function will get the information, only in the case of n_good + n_bad of the selected object is higher than nbr_move
    Return
        dico_information_by_number : DICTIONNARY containing rotation angle of each object sorted by object
    """
    dico_information_by_number = {"0":[],"1":[], "2":[], "3":[], "4":[], "5":[], "6":[], "7":[], "8":[], "9":[], "10":[],"11":[],"12":[],"13":[]}
    for key, value in dico.items():
        for line in value:
            if accepted_or_refuse in line:
                line = line.split(' ')
                if int(line[-2]) >= n_step_start:
                    if int(line[-2]) <= n_step_end:
                        n_good = int(line[10][:-1])
                        n_bad = int(line[7][:-1])
                        nbr = n_good + n_bad
                        if nbr > nbr_move:
                            rotation_angle = float(line[-5][:-1])
                            while rotation_angle > 2*math.pi:
                                rotation_angle -= 2*math.pi
                            while rotation_angle < 0:
                                rotation_angle += 2*math.pi
                            rotation_angle = rotation_angle*180/math.pi
                            dico_information_by_number[key].append(rotation_angle)
    return dico_information_by_number

def get_translation(dico, n_step_start, n_step_end, accepted_or_refuse, nbr_move):
    """
    This function take a dico from divided_in_dico() and will create a dico with the amplitude of translation of each object
    Parameters
        dico : DICTIONNARY containing all the information from the input file by sorted by object number
        n_step_start (filter): the function didn't get the information if the n_step in the logger is lower than n_step_start
        n_step_end(filter) : the function didn't get the information if the n_step in the logger is higher than n_step_end
        accepted_or_refuse (filter) : this filter is usefull to get only one type of move (to use it write "accepted" or "refused")
        nbr_move (filter) : the function will get the information, only in the case of n_good + n_bad of the selected object is higher than nbr_move
    Return
        dico_information_by_number : DICTIONNARY containing the amplitude of translation of each object sorted by object
    """
    dico_information_by_number = {"0":[],"1":[], "2":[], "3":[], "4":[], "5":[], "6":[], "7":[], "8":[], "9":[], "10":[],"11":[],"12":[],"13":[]}
    for key, value in dico.items():
        for line in value:
            if accepted_or_refuse in line:
                line = line.split(' ')
                if int(line[-2]) >= n_step_start:
                    if int(line[-2]) <= n_step_end:
                        n_good = int(line[10][:-1])
                        n_bad = int(line[7][:-1])
                        nbr = n_good + n_bad
                        if nbr > nbr_move:
                            dy = float(line[-7][:-1])
                            dx = float(line[-9][:-1])
                            temp = (dx*dx) + (dy*dy)
                            rotation_angle = math.sqrt(temp)
                            dico_information_by_number[key].append(rotation_angle)
    return dico_information_by_number

def get_ngood_bad(dico, n_step_start,n_step_end, accepted_or_refuse, nbr_move):
    """
    This function take a dico from divided_in_dico() and will get the MC score (n_good/n_good + n_bad)in a dico for each object
    Parameters
        dico : DICTIONNARY containing all the information from the input file by sorted by object number
        n_step_start (filter): the function didn't get the information if the n_step in the logger is lower than n_step_start
        n_step_end(filter) : the function didn't get the information if the n_step in the logger is higher than n_step_end
        accepted_or_refuse (filter) : this filter is usefull to get only one type of move (to use it write "accepted" or "refused")
        nbr_move (filter) : the function will get the information, only in the case of n_good + n_bad of the selected object is higher than nbr_move
    Return
        dico_information_by_number : DICTIONNARY containing the MC score of each object sorted by object
    """
    dico_information_by_number = {"0":[],"1":[], "2":[], "3":[], "4":[], "5":[], "6":[], "7":[], "8":[], "9":[], "10":[],"11":[],"12":[],"13":[]}
    for key, value in dico.items():
        for line in value:
            if accepted_or_refuse in line:
                line = line.split(' ')
                if int(line[-2]) >= n_step_start:
                    if int(line[-2]) <= n_step_end:
                        n_good = int(line[10][:-1])
                        n_bad = int(line[7][:-1])
                        nbr = n_good + n_bad
                        if nbr > nbr_move:
                            n_good = int(line[10][:-1])
                            n_bad = int(line[7][:-1])
                            try :
                                mobi = n_good/(n_good+n_bad)
                                dico_information_by_number[key].append(mobi)
                            except:
                                pass
    return dico_information_by_number


#===============================================================================

def draw_graph(dico, xlim, ylim):
    """
    This function take a dico at argument and return a density curve.
    Parameters
        dico : DICTIONNARY containing a specific information sorted by obj_number
        xlim : maximum value of the x axis in the graph
        ylim : maximum value of the y axis in the graph

    Return
        None, but generate a graph
    """
    figure, axis = plt.subplots(5, 3)
    columns = 0
    line = 0
    for key, value in dico.items():
        axis[line, columns].plot(value)
        axis[line, columns].set_xlim([0, xlim])
        axis[line, columns].set_ylim([0, ylim])
        columns += 1
        if columns > 2 :
            columns = 0
            line += 1
    plt.show()

def draw_hist(dico):
    """
    This function take a dico at argument and return an histograms to see the distribution.
    Parameters
        dico : DICTIONNARY containing a specific information sorted by obj_number
        xlim : maximum value of the x axis in the histograms
        ylim : maximum value of the y axis in the histograms

    Return
        None, but generate an histograms
    """
    figure, axis = plt.subplots(5, 3)
    columns = 0
    line = 0
    for key, value in dico.items():
        axis[line, columns].hist(value, 200)
        axis[line, columns].set_xlim([-10, 380])
        axis[line, columns].set_ylim([0, 10])
        columns += 1
        if columns > 2 :
            columns = 0
            line += 1
    plt.show()

if __name__ == "__main__":

    #THE FIRST STEP IS TO READ THE CONTENT AND SORT IN DICTIONNARY
    file_handler = read_content("/home/alexandre/github/hard_discs2-master/test_alex/NEW_algorithme_movement.txt")
    dico_of_each_object = divided_in_dico(file_handler)
    ##============================================================================================================="
#EXAMPLE :

        #get obj_dl_max of each object afte 25000 step
    #dico_dl_max = get_obj_dl_max(dico_of_each_object, 2000, 50000, "", 0)
    #draw_graph(dico_dl_max, 3000, 20)

        #get rotation of each object  n_step_start
    #dico_rotation = get_rotation(dico_of_each_object, 25000, 50000, "", 0)
    #draw_hist(dico_rotation)

        #get translation of each object  n_step_start
    #dico_translation = get_translation(dico_of_each_object, 0, 50000, "accepted", 0)
    #draw_graph(dico_translation, 100, 100)

        #get n_good, n_bad of each object  n_step_start
    #dico_n_good_n_bad = get_ngood_bad(dico_of_each_object, 40000, 50000, "", 0)
    #draw_graph(dico_n_good_n_bad, 3600, 1)
