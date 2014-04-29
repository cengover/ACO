#!/bin/bash
total=0
thresholdList=(0.2 0.4 0.6)
interventionList=(0.30 0.60 0.90)
make abm
for((c = 1; c <= 15; c+=3))
do
	for((p = 1; p<=10; p+=4))
	do
		for((b = 1; b<=10; b+=4))
		do
			for((n = 1; n<=21; n+=10))
			do
				for threshold in ${thresholdList[@]}
				do
					for((med = 10; med<=31; med+=10))
					do
						for intervention in ${interventionList[@]}
						do
							for ((run = 1; run <= 10;++run))
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
