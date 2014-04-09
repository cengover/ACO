/*
 * Observer.cpp

 *
 *  Created on: Mar 17, 2014
 *      Author: ozi
 */
#include "Observer.h"
#include "config.h"
using namespace std;
using namespace adevs;

// Assign a locally unique number to the input port
const int Observer::output_port = 0;

Observer::Observer():Atomic<IO>()
//output_strm(output_file)
{
	total_number_of_patients = 0;
}

double Observer::ta()
{
	// The Observer has no autonomous behavior, so its next event
	// time is always infinity.
	return DBL_MAX;
}

void Observer::delta_int()
{
	// The Observer has no autonomous behavior, so do nothing
}

void Observer::delta_ext(double e, const Bag<IO>& xb)
{
	// Record the times at which the bene left the provider.
	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++)
	{
		//const Signal* c = (*i).value;
		total_number_of_patients += 1;
	}
}

void Observer::delta_conf(const Bag<IO>& xb)
{
	// The Observer has no autonomous behavior, so do nothing
}

void Observer::output_func(Bag<IO>& yb)
{
	// The Observer produces no output, so do nothing
}

void Observer::gc_output(Bag<IO>& g)
{
	// The Observer produces no output, so do nothing
}

Observer::~Observer()
{
	// Close the statistics file
	// output_strm.close();
}
