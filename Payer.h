/*
 * Observer.h
 *
 *  Created on: Mar 17, 2014
 *      Author: ozi
 */

#ifndef PAYER_H_
#define PAYER_H_
#include "adevs.h"
#include "config.h"
#include <fstream>

/**
 * The Observer records performance statistics for a Clerk model
 * based on its observable output.
 */
class Payer: public adevs::Atomic<IO>
{
	public:
		/// Input port for receiving customers that leave the store.
		static const int output_port;
		/// Constructor.
		Payer();
		/// Internal transition function.
		void delta_int();
		/// External transition function.
		void delta_ext(double e, const adevs::Bag<IO>& xb);
		/// Confluent transition function.
		void delta_conf(const adevs::Bag<IO>& xb);
		/// Time advance function.
		double ta();
		/// Output function.
		void output_func(adevs::Bag<IO>& yb);
		/// Output value garbage collection.
		void gc_output(adevs::Bag<IO>& g);
		/// Destructor.
		~Payer();
		double lookahead(){

			return t_lookahead;
		}
		int total_number_of_patients;
	private:
		/// File for storing information about departing customers.
		//std::ofstream output_strm;
};

#endif /* PAYER_H_ */
