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
}

static int get_binary(){

	return ceil(rand_strm.uniform(0.0,1.0)-0.5);
}

static double get_uniform(double a, double b){

	return rand_strm.uniform(a,b);
}

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
	// initialize severity
	severity = 0.2*gene+0.2*behavior+0.2*intervention+0.2*health*0.25; //+0.20*t_cum/t;
	diagnosed = false;
}

/// Internal transition function.
void Bene::delta_int(){

	t += tahead;
	tahead = get_signal_time();
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

		//cout<<severity<<endl;
		// Update Severity
		if (t_conduct != 0 && t_cum > 0){

			severity = 0.2*gene+0.2*behavior+0.2*intervention+0.20*health*0.25+0.20*(t-t_conduct)/t_cum;
		}

	}
}

/// External transition function.
void Bene::delta_ext(double e, const adevs::Bag<IO>& xb){

	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++){

		// If the bene comes back from a provider
		if ((*i).port == 1 && hospitalized == 1&&(*i).value->id == this->id){

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
		}
	}
	t = t+e;

	if (e >= tahead){

			tahead = get_signal_time();
	}
	else{
			tahead = ta()-e;
	}
}

/// Confluent transition function.
void Bene::delta_conf(const adevs::Bag<IO>& xb){

	delta_int();
	delta_ext(0,xb);
}

/// Output function.
void Bene::output_func(adevs::Bag<IO>& yb){

	Signal* sig = new Signal();
	sig->health = health;
	sig->behavior = behavior;
	sig->id = id;
	sig->entry_time = 0;
	sig->diagnosed = diagnosed;
	sig->insurance = insurance;
	// Here we assign state transitions
	if (severity > threshold + 0.1*health){

		intervention = 0;
		if (health != 4){

			health = health + 1;
		}

		if (insurance == 1){

			hospitalized = 1;
			IO output(signal_out[1],sig);
			yb.insert(output);
		}

		else if (insurance ==0 && get_uniform(0.0,1.0)<0.5) {

			hospitalized = 1;
			IO output(signal_out[1],sig);
			yb.insert(output);
		}
	}
	else {

		IO output(signal_out[0],sig);
		yb.insert(output);
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




