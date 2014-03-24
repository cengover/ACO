/*
 * Provider.h
 *
 *  Created on: Mar 10, 2014
 *      Author: ozi
 */

#ifndef PROVIDER_H_
#define PROVIDER_H_

#include "adevs.h"
#include "config.h"
#include <vector>

class Provider: public adevs::Atomic<IO>
{
	public:
		/// Constructor.
		Provider();
		/// Internal transition function.
		void delta_int();
		/// External transition function.
		void delta_ext(double e, const adevs::Bag<IO>& xb);
		/// Confluent transition function.
		void delta_conf(const adevs::Bag<IO>& xb);
		/// Output function.
		void output_func(adevs::Bag<IO>& yb);
		/// Time advance function.
		double ta();
		/// Output value garbage collection.
		void gc_output(adevs::Bag<IO>& g);
		int id;
		double t;
		double busy_time;
		int total_patients;
		int distinct_patients;
		std::vector<Signal*> patients;
		static const int signal_in;
		static const int signal_out;
		static const int observer_out;
	private:

		double tahead;
};
#endif /* PROVIDER_H_ */
