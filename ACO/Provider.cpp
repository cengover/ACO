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
const int Provider::signal_in = 0;
const int Provider::signal_out = 1;
const int Provider::observer_out = 2;

/// Get next service duration
static double get_visit_time(){

	return rand_strm.exponential(provider_service_rate);
	//return 10.0;
}

/// Get binary random variable
static int get_binary(){

	return ceil(rand_strm.uniform(0.0,1.0)-0.5);
}

/// Create a signal
Signal* Provider::create_signal(){

	Signal* sig = new Signal();
	list<Signal*>::iterator iter = patients.begin();
	/// Add Output Signal Values
	sig->id = (*iter)->id;
	sig->health=0;
	sig->behavior = 0;
	sig->intervention = get_binary();

	///Intervention budget is incremented
	if (sig->intervention ==1){

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
	p->busy_time+=(p->t-(*iter)->entry_time);
	cout<<p->t<<" "<<(*iter)->id<<" Discharge"<<endl;
	p->patients.erase(p->patients.begin());
	if (p->patients.size()>0){

		iter = p->patients.begin();
		(*iter)->entry_time = p->t;
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

		// System entry time is recorded
		(*i).value->entry_time = t;
		(*i).value->service_duration = get_visit_time();
		// Add the patient to the list of patients
		patients.push_back((*i).value);
		cout<<t<<" "<<(*i).value->id<<" "<<(*i).value->service_duration<<" IN "<<endl;
	}
	if (e > tahead){

		Signal* s = new Signal();
		list<Signal*>::iterator sig = patients.begin();
		s = (*sig);
		tahead = s->service_duration;
	}
	else{

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
	IO output(signal_out, sig);
	yb.insert(output);
	IO output1(observer_out, sig);
	yb.insert(output1);
	t += tahead;
	// Remove the discharged bene
	discharge(this);
	if (patients.size() > 0){

		Signal* s = new Signal();
		list<Signal*>::iterator si = patients.begin();
		s = (*si);
		tahead = s->service_duration;
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

