-lancer le script (./make_test.sh)

permet de crée 1 folder contenant un test NVT avec chacun des 5 force field

- algorithme python permet de separer le fichier contenant la sauvegarde des coordonnées dans chaque folder en 500 fichier.conf

-le script poursuit en creant un fichier.eps pour chacun des 500 fichiers.conf crée.

-command pour convertir les 500 file.eps en 1 file.gif 
	convert -delay 50 -dispose Background -loop 2 *.eps animation.gif
	
	

