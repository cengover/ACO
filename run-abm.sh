#!/bin/bash
total=0
for((c = 1; c <= 1; c+=3))
do
	for((p = 0; p<=1; p+=1))
	do
		for((b = 1; b<=1; b+=1))
		do
			for ((run = 1; run <= 1;++run))
			do
				total=$(($total+1))
				echo $total
				export Seed=$run
				export connections=$c
				export provider_service_rate=$p
				export bene_signal_rate=$b
        			#make clean
        			make abm
				./abm  #> abm-$total.txt
done
done
done
done
#t=`echo "$en*1" | bc -l`
#echo $t
#sed "s/const double EncounterRate = 4.0;/const double EncounterRate
#= $t;/g" abmparse.txt > abm.cpp

