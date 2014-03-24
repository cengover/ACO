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

static double get_service_time(){

	return rand_strm.exponential(provider_service_rate);
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

	t += ta();
	// If there are not any patients waiting, do not delete
	if (patients.size()>0){

		if(patients[0]->serviced_before == false){

			this->distinct_patients += 1;
		}
		total_patients+=1;
		busy_time+=(t-patients[0]->service_time);
		patients.erase(patients.begin());
	}
	tahead = get_service_time();
}

/// External transition function.
void Provider::delta_ext(double e, const adevs::Bag<IO>& xb){

	t+=e;
	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++){

		(*i).value->service_time = t;
		patients.push_back((*i).value);
	}
	if (e >= tahead){

		tahead = get_service_time();
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
	/// Add Output Signal Values
	sig->id = patients[0]->id;
	sig->health_status = 0;
	sig->state = 0;
	IO output(signal_out, sig);
	yb.insert(output);
	IO output1(observer_out, sig);
	yb.insert(output1);
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



