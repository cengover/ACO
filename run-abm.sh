#!/bin/bash
total=0
thresholdList=(0.2)
interventionList=(0.2)
make abm
for((c = 5; c <= 5; c+=3))
do
	for((p = 1; p<=1; p+=3))
	do
		for((b = 1; b<=1; b+=3))
		do
			for((n = 1; n<=1; n+=10))
			do
				for threshold in ${thresholdList[@]}
				do
					for((med = 1; med<=1; med+=5))
					do
						for intervention in ${interventionList[@]}
						do
							for ((run = 1; run <= 30;++run))
							do
							total=$(($total+1))
							echo $total
							export Seed=$run
							export connections=$c
							export provider_service_rate=$p
							export bene_signal_rate=$b
							export numberOfProviders=$n
							export max_threshold=$threshold
							export medication_period=$med
							export intervention_rate=$intervention
							#make clean
							#make abm
							./abm  #> abm-$total.txt
done
done
done
done
done
done
done
done
#t=`echo "$en*1" | bc -l`
#echo $t
#sed "s/const double EncounterRate = 4.0;/const double EncounterRate
#= $t;/g" abmparse.txt > abm.cpp
