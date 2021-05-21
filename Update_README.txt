UPDATE recap

General update :
in object.cpp
  -add : obj_n_good & obj_n_bad to save a count of MC score of a specific object.
  -add : obj_n_rotation, obj_n_translation, to save a count of move of a specific object.
  
in integrator.cpp
  -add : n_try attribut to be able to use two differents move model (move behaviour)
  -modify : run() to use two differents move behaviour (primary_move & move_aftern_primary_move)
  -modify : run() to increase/reduce obj_n_good, obj_n_bad, obj_n_rotation, obj_n_translation of each objects.
  

To update the rotation :
in config.cpp
  -modify : move() become primary_move()
  -add : move_aftern_primary_move() currently an object move thanks to primary_move like the original algorithm  and after n_try the selected object will move thank's to move_aftern_primary_move()
  -add : trans_over_rot() function to make a choice between rotation and/or translation thanks to the MC score of an specific object.
  -add : rnd_rotate() it's a function that actually purpose quarter turn.
  -add : side_object() it's a function used by rnd_rotate().
  -add : modif_mobility() to append MC score to each objects and modify their dl_max after try to moving them.

To update the translation :
in object.cpp
  -add : obj_n_dl_max to fix a specific limit for a translation.
  
in integrator.cpp
  -add : initial_dl_max, it's an attribut to be able to reassign the initial dl_max of the system to each objects.
  -modify : run() to reassign initial_dl_max to each objects.
in NVT.cpp
  -modify : main() to set initial dl_max to each objects.

To save calculation time :
in config.cpp
  -add : need_calculate_energy() to check if we need to calculate all interaction when object move.
  -modify : energy() by adding a decision maker with need_calculation_energy()
in NVT.cpp
  -modify : main() to add an error message and default value when the r_list factor is negative or not define
 

To create a energy barrier :
in force_field.cpp
  -add : barrier attribut in force field
  -modify : read_force_field() to be able to read the value of barrier energy from force_field file
  -modify : interaction() to implement energy barrier concept
  
 
 To create a new log :
 in config.cpp
  - modify : primary_move() & move_aftern_primary_move() to read obj_n_good, obj_n_bad, obj_n_rotation, obj_n_translation of the selected object and the dx, dy (translation) and angle (rotation) of this move.
 in integrator.cpp
  - modify : run() to read the number of current steps and if the purposed move is accepted of refused.
