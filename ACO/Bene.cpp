/*

 * Bene.cpp
 *
 *  Created on: Feb 27, 2014
 *      Author: ozgur
 */
#include "Bene.h"
#include "config.h"
#include "adevs.h"
using namespace std;
using namespace adevs;

// Assign identifiers to I/O ports
const int Bene::signal_in[2] = {0,1};
const int Bene::signal_out[2]= {0,1};

static double get_signal_time(){

	return rand_strm.exponential(bene_signal_rate);
	//return bene_signal_rate;
}

static int get_binary(){

	return ceil(rand_strm.uniform(0.0,1.0)-0.5);
}

static double get_uniform(double a, double b){

	return rand_strm.uniform(a,b);
}

//////////////////////////// Construct Benes ///////////////////////////
Bene::Bene():Atomic<IO>(){

	t = 0;
	tahead = get_signal_time();
	t_conduct = 0;
	t_cum = 0;
	insurance = get_binary();
	behavior = get_binary();
	health = get_binary();
	gene = get_binary();
	hospitalized = 0;
	intervention = 0;
	id = 0;
	total = 1;
	influence = 1;
	threshold = get_uniform(0.0,0.05);
	risk_aversion = get_uniform(0.0,1.0);
	// initialize progression
	progression = 0;
	diagnosed = false;
}

/// Internal transition function.
void Bene::delta_int(){

	if (this->hospitalized == 0){
		// Change Behavior
		double temp = get_uniform(0.0,1.0);
		if (temp<=(influence/total)){

			if (behavior != 1){

				behavior = 1;
				t_conduct = t;
			}
		}
		else if (temp>(influence/total)){

			if (behavior != 0){

				behavior = 0;
				t_cum = t_cum + (t-t_conduct);
				t_conduct = 0;
			}
		}
		// Update progression
		update_progression();
	}
}

/// External transition function.
void Bene::delta_ext(double e, const adevs::Bag<IO>& xb){

	t += e;
	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++){

		// If the bene comes back from a provider
		if ((*i).port == 1 && hospitalized == 1&&(*i).value->id == this->id){

			tahead = get_signal_time();
			if (health == 1){

				health = 0;
			}
			hospitalized = 0;
			diagnosed = true;
			intervention = (*i).value->intervention;
		}
		// If the bene receives a signal from another bene
		else if ((*i).port == 0 && hospitalized != 1) {

			influence += (*i).value->behavior;
			total += 1;
			tahead = ta()-e;
		}
		else if ((*i).port == 1 && (*i).value->id != this->id){

			tahead = ta()-e;
		}
	}
}

/// Confluent transition function.
void Bene::delta_conf(const adevs::Bag<IO>& xb){

	delta_int();
	delta_ext(0,xb);
}

/// Output function.
void Bene::output_func(adevs::Bag<IO>& yb){

	t += tahead;
	Signal* sig = new Signal();
	sig->health = health;
	sig->behavior = behavior;
	sig->id = id;
	sig->entry_time = 0;
	sig->diagnosed = diagnosed;
	sig->insurance = insurance;
	// Here we assign state transitions
	if (progression > threshold + w_health_for_threshold*health){

		intervention = 0;
		// If health is not the last stage
		if (health != 4){

			health = health + 1;
			sig->health = health;
		}
		if (insurance == 1){

			hospitalized = 1;
			IO output(signal_out[1],sig);
			yb.insert(output);
		}
		// After we add self-efficacy, we will change the condition
		else if (insurance ==0 && get_uniform(0.0,1.0)>risk_aversion) {

			hospitalized = 1;
			IO output(signal_out[1],sig);
			yb.insert(output);
		}

		cout<<t<<" "<<id<<" "<<hospitalized<<endl;
		tahead = 0;
	}
	else {

		IO output(signal_out[0],sig);
		yb.insert(output);
		tahead = get_signal_time();
	}
}

/// Time advance function.
double Bene::ta(){

	if (hospitalized == 1){

		return DBL_MAX;
	}
	else {

		return tahead;
	}
}

/// Output value garbage collection.
void Bene::gc_output(adevs::Bag<IO>& g){
};
void Bene::update_progression(){

	if (t_conduct != 0 && t_cum > 0){

		progression = weights_in_progression[0]*gene+weights_in_progression[1]*behavior
				+weights_in_progression[2]*intervention+weights_in_progression[3]*health/4+
				weights_in_progression[4]*(t-t_conduct)/t_cum;
	}
	else{

		progression = weights_in_progression[0]*gene+weights_in_progression[1]*behavior
				+weights_in_progression[2]*intervention+weights_in_progression[3]*health/4;
	}

}



