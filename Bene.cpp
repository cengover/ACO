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

static double get_exponential(double i){

	//return rand_strm.exponential(i);
	return i;
}

static int get_binary(){

	return ceil(rand_strm.uniform(0.0,1.0)-0.5);
}

static double get_uniform(double a, double b){

	return rand_strm.uniform(a,b);
}

Bene::Bene():Atomic<IO>(){

	t = 0;
	tahead = get_exponential(bene_signal_rate);
	t_conduct = 0;
	t_cum = 0;
	t_hospital = 0;
	t_queue = 0;
	duration = 0;
	insurance = get_binary();
	lifestyle = get_binary();
	health = get_binary();
	gene = get_binary();
	hospitalized = 0;
	intervention = 0;
	diagnosed = false;
	id = 0;
	total = 1;
	influence = 1;
	threshold = get_uniform(0.0,max_threshold);
	risk_aversion = get_uniform(0.0,1.0);
	memory = 5;
	memory_count = 0;
	memory_factor = get_uniform(0.0,0.2);
	tendency = 0;
	progression = 0;
}

/// Internal transition function.
void Bene::delta_int(){

	if (this->hospitalized == 0){
		// Change lifestyle
		double temp = get_uniform(0.0,1.0);
		// Revise the tendency
		memory_count++;
		tendency = memory_factor*(influence/total)+(1-memory_factor)*tendency;
		if (memory_count == memory){

			memory_count = 0;
			influence = 1;
			total = 1;
		}
		if (temp<=tendency){

			if (lifestyle != 1){

				lifestyle = 1;
				t_conduct = t;
			}
		}
		else if (temp>tendency){

			// Update duration
			if (lifestyle != 0){

				this->lifestyle = 0;
				t_cum = t_cum + (t-t_conduct);
				// Update Duration
				duration = (double)t_cum/(t-t_hospital);
				t_conduct = t;
			}
		}
		update_progression();
	}
}

/// External transition function.
void Bene::delta_ext(double e, const adevs::Bag<IO>& xb){

	t += e;
	tahead = ta()-e;
	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++){
		// If the bene comes back from a provider
		if ((*i).value->from_provider == 1 && id ==(*i).value->id){

			tahead = get_exponential(medication_period);
			if (health == 1){

				health = 0;
			}
			hospitalized = 0;
			lifestyle = 0;
			diagnosed = true;
			intervention = (*i).value->intervention;
			t_hospital = t_hospital + (t - (*i).value->entry_time);
			t_queue = t_queue + (*i).value->t_queue;
		}
		// If the bene receives a signal from another bene
		else if ((*i).value->from_bene == 1 && hospitalized != 1) {
			influence += (*i).value->lifestyle;
			total += 1;
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

	if (lifestyle != 0){

		t_cum = t_cum + (t-t_conduct);
		// Update Duration
		duration = (double)t_cum/(t-t_hospital);
		t_conduct = t;
	}
	else if(lifestyle == 0){
		// Update Duration
		duration = (double)t_cum/(t-t_hospital);
	}
	update_progression();
	// Here we assign health status transitions
	if (progression > threshold + w_health_for_threshold*health){

		intervention = 0;
		// If health is not the last stage
		if (health != 3){

			health = health + 1;
		}
		vector<int>::iterator s  = signal_to_provider.begin();
		if (insurance == 1){

			hospitalized = 1;
			Signal* sig = create_signal_to_provider();
			IO output((*s),sig);
			yb.insert(output);
			update_duration();
		}
		// After we add self-efficacy, we will change the condition
		else if (insurance ==0 && get_uniform(0.0,1.0)>risk_aversion){

			hospitalized = 1;
			Signal* sig = create_signal_to_provider();
			IO output((*s),sig);
			yb.insert(output);
		}
		tahead = 0;
	}
	// Here interact with other agents
	else {

		vector<int>::iterator b  = this->signal_to_bene.begin();
		for(;b!=signal_to_bene.end();b++){

			Signal* sig = create_signal_to_bene();
			IO output((*b),sig);
			yb.insert(output);
		}
		tahead = get_exponential(bene_signal_rate);
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

	// Delete the outgoing signal objects
	Bag<IO>::iterator i;
	for (i = g.begin(); i != g.end(); i++)
	{
		delete (*i).value;
	}
}

/// Update progression
void Bene::update_progression(){

	duration = (double)t_cum/(t-t_hospital);
	if (t_cum > 0){

		progression = weights_in_progression[0]*gene+weights_in_progression[1]*lifestyle
				-weights_in_progression[2]*intervention+weights_in_progression[3]*health/3+
				weights_in_progression[4]*duration;
	}
	else{

		progression = weights_in_progression[0]*gene+weights_in_progression[1]*lifestyle
				-weights_in_progression[2]*intervention+weights_in_progression[3]*health/3;
	}
	if (progression < 0){

		progression = 0;
	}
}

/// Update duration
void Bene::update_duration(){

	if (lifestyle != 0){

		t_cum = t_cum + (t-t_conduct);
		// Update Duration
		duration = (double)t_cum/(t-t_hospital);
		t_conduct = t;
	}
}

/// Create signals for bene to bene interaction
Signal* Bene::create_signal_to_bene(){

	Signal* signal = new Signal();
	// Add Output Signal Values
	signal->id = id;
	signal->health = health;
	signal->lifestyle = lifestyle;
	signal->diagnosed = diagnosed;
	signal->insurance = insurance;
	signal->from_bene = 1;
	return signal;
}

/// Create signals for bene to provider interaction
Signal* Bene::create_signal_to_provider(){

	Signal* signal = new Signal();
	// Add Output Signal Values
	signal->id = id;
	signal->health = health;
	signal->lifestyle = lifestyle;
	signal->diagnosed = diagnosed;
	signal->insurance = insurance;
	signal->to_provider = 1;
	return signal;
}
