/*
 * Bene.h
 *
 *  Created on: Feb 27, 2014
 *      Author: ozgur
 */
#ifndef bene_h_
#define bene_h_
#include "adevs.h"
#include "config.h"

class Bene: public adevs::Atomic<IO>
{
	public:
		/// Constructor.
		Bene();
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
		int insurance; // 0 = Exist, 1 = Not-exist
		int behavior; // 0 = Un-Healthy, 1 = Healthy
		int health; // 0 = Healthy, 1 = Sick
		int hospitalized; // 0 = Not Hospitalized, 1 = At Hospital
		int id;
		bool hospitalized_before; // 0 = Not hospitalized before, 1 = Hospitalized before
		// Model input port
		static const int signal_in[2];
		// Model output port
		static const int signal_out[2];
		double t;

	private:

		double tahead;
		int total;
		int influence;
		int memory;
		double memory_count;


};

#endif /* BENE_H_ */
