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

// Assign identifiers to I/O ports
const int Provider::signal_in = 0;
const int Provider::signal_out = 1;
const int Provider::observer_out = 2;

static double get_visit_time(){

	return rand_strm.exponential(provider_service_rate);
	//return 10.0;
}

// Discharge Function
void Provider::discharge(Provider* p){

	list<Signal*>::iterator iter = p->patients.begin();
	if((*iter)->hospitalized_before == false){

		p->distinct_patients += 1;
	}
	p->total_patients+=1;
	p->busy_time+=(p->t-(*iter)->entry_time);
	p->patients.erase(p->patients.begin());
	if (p->patients.size()>0){

		iter = p->patients.begin();
		(*iter)->entry_time = p->t;
	}
}

Provider::Provider():Atomic<IO>(){

	t = 0;
	tahead = 0;
	id = 0;
	total_patients = 0;
	busy_time = 0;
	distinct_patients = 0;
}

/// Internal transition function.
void Provider::delta_int(){

	// If there are not any patients waiting, do not delete
	if (patients.size()>0){

		tahead = get_visit_time();
		//ta();
	}
}

/// External transition function.
void Provider::delta_ext(double e, const adevs::Bag<IO>& xb){

	t+=e;
	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++){

		(*i).value->entry_time = t;
		patients.push_back((*i).value);
	}
	if (e >= tahead){

		tahead = get_visit_time();
	}
	else{

		tahead = ta()-e;
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
	// Remove the discharged bene
	t += ta();
	cout<<t<<endl;
	discharge(this);
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


Signal* Provider::create_signal(){

	Signal* sig = new Signal();
	list<Signal*>::iterator iter = patients.begin();
	Signal* b = new Signal();
	b = *iter;
	/// Add Output Signal Values
	sig->id = b->id;
	sig->health= 0;
	sig->behavior = 0;
	return sig;
}
/// Output value garbage collection.
void Provider::gc_output(adevs::Bag<IO>& g){

};

