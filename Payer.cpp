/*
 * Payer.cpp

 *
 *  Created on: Mar 17, 2014
 *      Author: ozi
 */
#include "Payer.h"
#include "config.h"
using namespace std;
using namespace adevs;

// Assign a locally unique number to the input port
const int Payer::output_port = 0;

Payer::Payer():Atomic<IO>()
//output_strm(output_file)
{
	total_number_of_patients = 0;
}

double Payer::ta()
{
	// The Observer has no autonomous behavior, so its next event
	// time is always infinity.
	return DBL_MAX;
}

void Payer::delta_int()
{
	// The Observer has no autonomous behavior, so do nothing
}

void Payer::delta_ext(double e, const Bag<IO>& xb)
{
	// Record the times at which the bene left the provider.
	Bag<IO>::const_iterator i;
	for (i = xb.begin(); i != xb.end(); i++)
	{
		//const Signal* c = (*i).value;
		total_number_of_patients += 1;
	}
}

void Payer::delta_conf(const Bag<IO>& xb)
{
	// The Observer has no autonomous behavior, so do nothing
}

void Payer::output_func(Bag<IO>& yb)
{
	// The Observer produces no output, so do nothing
}

void Payer::gc_output(Bag<IO>& g)
{
	// The Observer produces no output, so do nothing
}

Payer::~Payer()
{
// If there will be an output file you can close it here...
}
