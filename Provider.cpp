/*
 * Provider.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: ozi
 */
#include "Provider.h"
#include "config.h"
#include "adevs.h"
using namespace std;
using namespace adevs;

/// Assign identifiers to I/O ports
const int Provider::payer_out = population+1;

/// To get next service duration
static double get_exponential(double i){

	return rand_strm.exponential(i);
}

/// Get binary random variable
static int random_process(double rate){

	double r = rand_strm.uniform(0.0,1.0);
	if (r < rate){

		return 1;
	}
	else{

		return 0;
	}
}

/// Constructor
Provider::Provider():Atomic<IO>(){

	t = 0;
	tahead = 0;
	id = 0;
	busy_time = 0;
	total_patients = 0;
	distinct_patients = 0;
	service_cost = 0;
	intervention_budget = 0;
}

/// Internal transition function.
void Provider::delta_int(){

	// Nothing for now
}

/// External transition function.
void Provider::delta_ext(double e, const adevs::Bag<IO>& xb){

	t+=e;
	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++){

		if ((*i).value->to_provider == 1){

			// System entry time is recorded
			(*i).value->entry_time = t;
			(*i).value->service_duration = get_exponential(provider_service_rate);
			(*i).value->t_queue = 0;
			// Add the patient to the list of patients
			patients.push_back(new Signal(*(*i).value));
		}
	}
	if (e > tahead){

		list<Signal*>::iterator s = patients.begin();
		tahead = (*s)->service_duration;
	}
	else {

		tahead = tahead-e;
	}
}

/// Confluent transition function.
void Provider::delta_conf(const adevs::Bag<IO>& xb){

	delta_int();
	delta_ext(0,xb);
}

/// Output function.
void Provider::output_func(adevs::Bag<IO>& yb){

	t += tahead;
	vector<int>::iterator it  = this->signal_out.begin();
	list<Signal*>::iterator iter = patients.begin();
	for(;it!=signal_out.end();it++){
		if ((*it) == (*iter)->id){
			// Create distinct signal for each output
			Signal* sig = create_signal_to_bene();
			IO output((*it), sig);
			yb.insert(output);
			// Remove the discharged bene
			discharge(this);
			break;
		}
	}
	// If Payer will be notified
	// signal->to_payer = 1;
	// IO output1(payer_out, sig);
	// yb.insert(output1);
	if (patients.size() > 0){

		list<Signal*>::iterator si = patients.begin();
		tahead = (*si)->service_duration;
		(*si)->t_queue = t-(*si)->entry_time;
	}
	else {

		tahead = 0;
	}
}

/// Time advance function.
double Provider::ta(){

	if (patients.size() == 0){

		return DBL_MAX;
	}
	else{

		return tahead;
	}
}

/// Output value garbage collection.
void Provider::gc_output(adevs::Bag<IO>& g){

	// Delete the outgoing signal objects
	Bag<IO>::iterator i;
	for (i = g.begin(); i != g.end(); i++)
	{
		delete (*i).value;
	}
}

Provider::~Provider()
{
	// Delete anything remaining in the customer queue
	list<Signal*>::iterator i;
	for (i = patients.begin(); i != patients.end(); i++)
	{
		delete *i;
	}
}

/// Create a signal for provider to bene interaction
Signal* Provider::create_signal_to_bene(){

	Signal* signal = new Signal();
	// Get the first customer in the queue
	list<Signal*>::iterator iter = patients.begin();
	// Add Output Signal Values
	signal->id = (*iter)->id;
	signal->intervention = random_process(intervention_rate);
	signal->from_provider = 1;
	signal->service_duration= (*iter)->service_duration;
	signal->entry_time = (*iter)->entry_time;
	signal->t_queue = (*iter)->t_queue;

	// Intervention budget is incremented
	if (signal->intervention == 1){

		this->intervention_budget += 0.1;
	}
	return signal;
}

/// Discharge Function
void Provider::discharge(Provider* p){

	list<Signal*>::iterator iter = p->patients.begin();
	if((*iter)->diagnosed == false){

		p->distinct_patients += 1;
	}
	p->total_patients+=1;
	p->service_cost += (*iter)->health*((*iter)->insurance+1);
	p->busy_time+=(*iter)->service_duration;
	delete *iter;
	p->patients.erase(iter);
}
