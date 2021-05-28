import os


for y in range(1,6):
    input_file = open("/home/alexandre/github/hard_discs2-master/test_alex/test_encomb_10/test_attract_"+str(y)+"/test2", "r")
    file_handler = input_file.readlines()
    line_index = 0
    list = []
    index_file = 0
    print(len(file_handler))
    for line in file_handler:
        line_index += 1
        if line_index > 17:
            line_index = 1
            list = []
        if line_index > 1 :
            list.append(line)
        if line_index == 17:
            try:
                os.makedirs("/home/alexandre/github/hard_discs2-master/test_alex/test_encomb_10/test_attract_"+str(y)+"/output_folder/eps_output_folder/")
            except OSError:
                print ("Creation of the directory %s failed" % "/home/alexandre/github/hard_discs2-master/test_alex/test_encomb_10/test_attract_"+str(y)+"/output_folder")
            else:
                print ("Successfully created the directory %s" % "/home/alexandre/github/hard_discs2-master/test_alex/test_encomb_10/test_attract_"+str(y)+"/output_folder")
            if index_file < 10 :
            	output_file = open("/home/alexandre/github/hard_discs2-master/test_alex/test_encomb_10/test_attract_"+str(y)+"/output_folder/conf00"+str(index_file)+".conf", "w")
            elif index_file > 99 :
            	output_file = open("/home/alexandre/github/hard_discs2-master/test_alex/test_encomb_10/test_attract_"+str(y)+"/output_folder/conf"+str(index_file)+".conf", "w")
            else :
           	    output_file = open("/home/alexandre/github/hard_discs2-master/test_alex/test_encomb_10/test_attract_"+str(y)+"/output_folder/conf0"+str(index_file)+".conf", "w")

            index_file += 1
            for line in list:
                output_file.write(line)
            output_file.close()
