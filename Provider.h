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
#include <list>

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
		/// Destructor.
		~Provider();
		/// To output a signal
		Signal* create_signal_to_bene();
		/// To discharge serviced patient
		void discharge(Provider* p);
		/// Lookahead
		double lookahead(){

			return t_lookahead;
		}
		int id; // Identifier of bene
		double t; // Synchronized with Simulation Time
		double busy_time; // Total number of busy time serving
		double service_cost; // Total service cost
		double intervention_budget; // Cumulative intervention cost
		int total_patients; // Cumulative number of patients served
		int distinct_patients; // Distinct number of patients who are served
		// List of patients in the line
		std::list<Signal*> patients;
		// Model input/output ports
		std::vector<int>  signal_in;
		std::vector<int> signal_out;
		static const int payer_out;

	private:

		double tahead; // Time to next event
};

#endif /* PROVIDER_H_ */
