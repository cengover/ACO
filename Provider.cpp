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

/// Get next service duration
static double get_visit_time(){

	return rand_strm.exponential(provider_service_rate);
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

/// Create a signal
Signal* Provider::create_signal(){

	Signal* sig = new Signal();
	list<Signal*>::iterator iter = patients.begin();
	/// Add Output Signal Values
	sig->id = (*iter)->id;
	sig->intervention = random_process(intervention_rate);
	sig->from_provider = 1;
	sig->service_duration= (*iter)->service_duration;
	sig->entry_time = (*iter)->entry_time;
	sig->t_queue = (*iter)->t_queue;

	// Intervention budget is incremented
	if (sig->intervention == 1){

		this->intervention_budget += 0.1;
	}
	return sig;
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
	p->patients.erase(p->patients.begin());
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
			(*i).value->service_duration = get_visit_time();
			(*i).value->t_queue = 0;
			// Add the patient to the list of patients
			patients.push_back((*i).value);
		}
	}
	if (e > tahead){

		list<Signal*>::iterator sig = patients.begin();;
		tahead = (*sig)->service_duration;
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

	Signal* sig = new Signal();
	sig = create_signal();
	vector<int>::iterator it  = this->signal_out.begin();
	for(;it!=signal_out.end();it++){
		if ((*it) == sig->id){

			IO output((*it), sig);
			yb.insert(output);
			break;
		}
	}
	// If Payer will be notified
	// sig->to_payer = 1;
	// IO output1(payer_out, sig);
	// yb.insert(output1);
	t += tahead;
	// Remove the discharged bene
	discharge(this);
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

};

