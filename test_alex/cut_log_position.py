# Python 3
from tkinter import Button
from tkinter import Label
from tkinter import StringVar
from tkinter import Tk
import os, os.path
import tkinter.filedialog

def FNC_Selection(nbr_obj) :
    path = tkinter.filedialog.askopenfilename ( title = "Search log file")
    print(path)
    input_file = open(path, "r")
    file_handler = input_file.readlines()
    line_index = 0
    list = []
    index_file = 0
    path_split = path.split('/')
    path_split = path_split[:-1]
    path = "/".join(path_split)
    print(path)
    for line in file_handler:
        line_index += 1
        if line_index > (nbr_obj + 3):
            line_index = 1
            list = []
        if line_index > 1 :
            list.append(line)
        if line_index == (nbr_obj + 3):
            try:
                os.makedirs(path + "/gif_folder")
                print(path)
            except OSError:
                print ("Creation of the directory failed")
            else:
                print ("Successfully created the directory")
            if index_file < 10 :
            	output_file = open(path + "/gif_folder/conf00"+str(index_file)+".conf", "w")
            elif index_file > 99 :
            	output_file = open(path + "/gif_folder/conf"+str(index_file)+".conf", "w")
            else :
           	    output_file = open(path + "/gif_folder/conf0"+str(index_file)+".conf", "w")

            index_file += 1
            for line in list:
                output_file.write(line)
            output_file.close()


nbr_obj = 2

TKI_Principal = tkinter.Tk ( )
TKI_Principal.geometry("200x200")

BUT_Quitter = tkinter.Button ( TKI_Principal , text = "Quitter" , command = TKI_Principal.destroy )
BUT_Selectionner = tkinter.Button ( TKI_Principal , text = "Sélectionner ... " , command = FNC_Selection(nbr_obj))

BUT_Selectionner.pack ( )
BUT_Quitter.pack ( )

TKI_Principal.mainloop ( )
