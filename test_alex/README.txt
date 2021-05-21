Run the script (./make_test.sh)
	-This script will create folders thank's to multiple force field and size of the box.
	-This script use a python algorythm to split log file (containing the saved position of objects.)
	-after using the python algorithm, the script will create eps from configuration & finally concat all the eps picture in GIF.

-Note : command to convert file.eps into file.gif 
	convert -delay 50 -dispose Background -loop 2 *.eps animation.gif
	
Cut_log.py is a python to cut a specific log file (containing the saved position of objects.)
	
	

