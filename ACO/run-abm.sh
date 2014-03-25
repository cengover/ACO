#!/bin/bash
total=0
#for ((en=18 ; en <= 20 ; en++))
#do
#	t=`echo "$en*1" | bc -l`
#	echo $t
#	sed "s/const double EncounterRate = 4.0;/const double EncounterRate 
#= $t;/g" abmparse.txt > abm.cpp

for ((run=1 ; run <= 30 ; run++))
do
	total=$(($total+1))
	echo $total
	export Seed=$run
        make clean
        make abm
	./abm $run  > abm-$total.txt
done
#done

