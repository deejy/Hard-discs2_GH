from Function_analysis_logger import *




if __name__ == "__main__":
    file_handler_new = read_content("/home/alexandre/github/hard_discs2-master/test_alex/NEW_algorithme_movement.txt")
    dico_of_each_object_new = divided_in_dico(file_handler_new)

    file_handler_old = read_content("/home/alexandre/github/OLD_folder_LISM/hard_discs2-master/test_alex/OLD_algorithme_movement.txt")
    dico_of_each_object_old = divided_in_dico(file_handler_old)
    ##============================================================================================================="

        #get obj_dl_max of each object afte 25000 step
    #dico_dl_max = get_obj_dl_max(dico_of_each_object_new, 0, 50000, "accepted", 0)
    #draw_graph(dico_dl_max, 3000, 20)

    #dico_dl_max = get_obj_dl_max(dico_of_each_object_old, 0, 50000, "", 0)
    #draw_graph(dico_dl_max, 3000, 20)
################################################################################
        #get rotation of each object  n_step_start
    #dico_rotation = get_rotation(dico_of_each_object_new, 0, 50000, "", 0)
    #draw_hist(dico_rotation)

    #dico_rotation = get_rotation(dico_of_each_object_old, 0, 50000, "", 0)
    #draw_hist(dico_rotation)
################################################################################
        #get translation of each object  n_step_start
    #dico_translation = get_translation(dico_of_each_object_new, 0, 50000, "", 0)
    #draw_graph(dico_translation, 700, 100)

    #dico_translation = get_translation(dico_of_each_object_old, 0, 50000, "", 0)
    #draw_graph(dico_translation, 700, 100)
################################################################################
        #get n_good, n_bad of each object  n_step_start
    #dico_n_good_n_bad = get_ngood_bad(dico_of_each_object_new, 0, 50000, "", 0)
    #draw_graph(dico_n_good_n_bad, 3600, 1)

    #dico_n_good_n_bad = get_ngood_bad(dico_of_each_object_old, 0, 50000, "", 0)
    #draw_graph(dico_n_good_n_bad, 3600, 1)
