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
	insurance = get_binary();
	behavior = get_binary();
	health = get_binary();
	hospitalized = 0;
	id = 0;
	total = 1;
	influence = 1;
	memory = 0;
	memory_count = ceil(get_uniform(1.0,5.0)-0.5);
	hospitalized_before = false;
}

/// Internal transition function.
void Bene::delta_int(){

	t += tahead;
	tahead = get_signal_time();
	if (this->hospitalized == 0){

		double temp = get_uniform(0.0,1.0);
		if (temp<=(influence/total)){

			behavior = 1;

		}
		else if (temp>(influence/total)){

			behavior = 0;
		}

		if ((influence/total)< influence_rate){

			memory+=1;
		}

		if (memory > memory_count){

			memory = 0;
			// get sick
			health = 1;
		}

	}
}

/// External transition function.
void Bene::delta_ext(double e, const adevs::Bag<IO>& xb){

	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++){

		// If the bene comes back from a provider
		if ((*i).port == 1 && hospitalized == 1&&(*i).value->id == this->id){

			health = 0;
			hospitalized = 0;
			hospitalized_before = true;
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
	sig->hospitalized_before = hospitalized_before;

	if (this->health == 1){

		hospitalized = 1;
		IO output(signal_out[1],sig);
		yb.insert(output);
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




