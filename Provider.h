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
		/// To output a signal, create one
		Signal* create_signal();
		/// To discharge serviced patient
		void discharge(Provider* p);
		/// Lookahead
		double lookahead(){

			return t_lookahead;
		}
		int id;
		double t;
		double busy_time;
		double service_cost;
		double intervention_budget;
		int total_patients;
		int distinct_patients;
		//int resources;
		//std::list<Signal*> in_service;
		std::list<Signal*> patients;
		std::vector<int>  signal_in;
		std::vector<int> signal_out;
		static const int payer_out;
	private:

		double tahead;
};

#endif /* PROVIDER_H_ */
