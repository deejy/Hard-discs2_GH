Run the script (./make_test.sh)
	-This script will create folders thank's to multiple force field and size of the box.
	-This script use a python algorythm to split log file (containing the saved position of objects.)
	-after using the python algorithm, the script will create eps from configuration & finally concat all the eps picture in GIF.

-Note : command to convert file.eps into file.gif 
	convert -delay 50 -dispose Background -loop 2 *.eps animation.gif
	
Cut_log.py is a python to cut a specific log file (containing the saved position of objects.)
	
topo3eps.dat : is a functional file containing 3 differents shapes to create objects (hard-disc, square, square+)
topo3eps_9_atoms.dat : is a functional file who only containing square+ shape.
	
how to do a simulation thanks to the required files :

     ../makeconfig/makeconfig -p -t topo3eps.dat -o test_1.conf -f 9_atoms.ff 10 10 2
     
     ../NVT/NVT -p -t topo3eps.dat -f 9_atoms.ff -o test_2.conf -c test_1.conf -l test2.log -n 100 -s test2.gz -r 1 50000 20 1 1
  
     ../config2eps/config2eps -t topo3eps.dat < test_1.conf > test_1.eps
     ../config2eps/config2eps -t topo3eps.dat < test_2.conf > test_2.eps
