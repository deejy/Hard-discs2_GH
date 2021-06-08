import os
from matplotlib import pyplot as plt
import numpy as np
from math import sqrt

##This algorithm allows to calculate the distance between 2 proteins with the information on the logger (file.log V1) 

def read_content(path):
    """
    read from logger and return it like a list.
    parameters : 
        path : path to read logger
    return :
        list containing the read file, line by line.
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

def delete_useless_content(list):
    """
    this function get the list from read_content() and delete the useless
    information needed to  calculate the distance between two objects.
    parameters :
        list : list from read_content()
    Return :
        list_cleaned : the same list but with only the information (position of each objects) to calculate distance between two proteins.
    """
    line_index = 0
    list_cleaned = []
    for line in list:
        line_index += 1
        if line_index > 3 :
            list_cleaned.append(line)
        if line_index  == 5 :
            line_index = 0
    return list_cleaned

def clean_list(list):
    """
    take a list and return it without empty element
    parameters :
        list : a list python
    Return :
        list_cleaned : the same list but without [""].
    """
    cleaned_list = []
    for element in list:
        if element != "":
            cleaned_list.append(element)
    return cleaned_list

def get_distance_obj(list, rayon_obj):
    """
    Calculate the distance between 2 object.
    Parameters :
        list : a list with the position of objects
    Return :
        list_distance : a list of the distance between the two proteins.
    """
    index_obj = 0
    index_calcul = 0
    list_distance = []
    obj1_dx = float(0)
    obj1_dy = float(0)
    obj2_dx = float(0)
    obj2_dy = float(0)
    for line in list:
        if index_obj > 1:
            index_calcul = 0
            index_obj = 0
            obj1_dx = float(0)
            obj1_dy = float(0)
            obj2_dx = float(0)
            obj2_dy = float(0)
        if index_obj == 0:
            index_calcul += 0.5
            obj1 = line
            obj1 = obj1.split(' ')
            obj1 = clean_list(obj1)
            obj1_dx = float(obj1[1])
            print(obj1)
            obj1_dy = float(obj1[2])

        if index_obj == 1:
            index_calcul += 0.5
            obj2 = line
            obj2 = obj2.split(' ')
            obj2 = clean_list(obj2)
            obj2_dx = float(obj2[1])
            obj2_dy = float(obj2[2])

        if index_calcul == 1 :
            distance = (obj2_dx - obj1_dx)**2 + (obj2_dy - obj1_dy)**2
            distance = sqrt(distance)
            list_distance.append(distance-(rayon_obj*2))
        index_obj += 1
    return list_distance

def create_histograms_from_list(data):
    # fixed bin siz fixed bin size
    """
    get a list of number and create an histograms
    Parameters :
        data : a list of number
    Return :
        None
    """

    plt.xlim([-1, 15])
    plt.ylim([0, 450])
    plt.hist(data, 50)
    plt.title('distribution of distance between object')
    plt.xlabel('relative size')
    plt.ylabel('count')

    plt.show()


if __name__ == "__main__":
    file_handler = read_content("/home/alexandre/github/hard_discs2-master/test_alex/test_encomb_10/test_attract_1/test2")
    dist_obj = delete_useless_content(file_handler)
    rayon_obj_max = 1
    list_distance = get_distance_obj(dist_obj, rayon_obj_max)
    print(list_distance)
    create_histograms_from_list(list_distance)
