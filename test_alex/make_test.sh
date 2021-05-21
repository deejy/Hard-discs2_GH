#!/bin/bash

#initialise the time
res1=$(date +%s.%N)

# do stuff in here
for i in 10 
do
mkdir test_encomb_$i
rm NEW_algorithme_movement.txt
 for x in 1 #`seq 1 5`
 do 
 mkdir test_encomb_$i/test_attract_$x
 ../makeconfig/makeconfig -p -t topo3eps_9_atoms.dat -o test_encomb_$i/test_attract_$x/test_1.conf -f 9_atoms_attract_$x.ff $i $i 2
 ../NVT/NVT -p -t topo3eps_9_atoms.dat -f 9_atoms_attract_$x.ff -o test_encomb_$i/test_attract_$x/test_2.conf -c test_encomb_$i/test_attract_$x/test_1.conf -l test_encomb_$i/test_attract_$x/test2.log -n 100 -s test_encomb_$i/test_attract_$x/test2.gz 50000 20 1 1
  gunzip test_encomb_$i/test_attract_$x/test2.gz
  for n in `seq 1 2`
  do
  ../config2eps/config2eps -t topo3eps_9_atoms.dat < test_encomb_$i/test_attract_$x/test_$n.conf > test_encomb_$i/test_attract_$x/test$n.eps
  done
 done
done

python3 test_p.py

for i in 10
do
 for x in 1 #`seq 1 5`
 do 
  for n in `seq 0 999`
  do
    ../config2eps/config2eps -t topo3eps_9_atoms.dat < test_encomb_$i/test_attract_$x/output_folder/conf$n.conf > test_encomb_$i/test_attract_$x/output_folder/eps_output_folder/conf$n.eps
    ../config2eps/config2eps -t topo3eps_9_atoms.dat < test_encomb_$i/test_attract_$x/output_folder/conf00$n.conf > test_encomb_$i/test_attract_$x/output_folder/eps_output_folder/conf00$n.eps 
    ../config2eps/config2eps -t topo3eps_9_atoms.dat < test_encomb_$i/test_attract_$x/output_folder/conf0$n.conf > test_encomb_$i/test_attract_$x/output_folder/eps_output_folder/conf0$n.eps 
  done
  convert -delay 40 -dispose Background -loop 2 test_encomb_$i/test_attract_$x/output_folder/eps_output_folder/*.eps test_encomb_$i/animation$x.gif
 done
done




#get the final time and show it 
res2=$(date +%s.%N)
dt=$(echo "$res2 - $res1" | bc)
dd=$(echo "$dt/86400" | bc)
dt2=$(echo "$dt-86400*$dd" | bc)
dh=$(echo "$dt2/3600" | bc)
dt3=$(echo "$dt2-3600*$dh" | bc)
dm=$(echo "$dt3/60" | bc)
ds=$(echo "$dt3-60*$dm" | bc)

LC_NUMERIC=C printf "Total runtime: %d:%02d:%02d:%02.4f\n" $dd $dh $dm $ds
