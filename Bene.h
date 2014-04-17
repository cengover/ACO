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
#include "Provider.h"
#include <list>
#include <vector>

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
		/// Update Progression
		void update_progression();
		/// Lookahead
		double lookahead(){

			return t_lookahead;
		}
		int id;
		int insurance; // 0 = Exist, 1 = Not-exist
		int behavior; // 0 = Un-Healthy, 1 = Healthy
		int health; // 0 = Healthy, 1 = Pre-Diabetes, 2 = Diabetes, 3 = Insulin Dependence, 4 = Death
		int hospitalized; // 0 = Not Hospitalized, 1 = At Hospital
		int intervention;// 1 = Received phone from hospital, 0 = Not received follow up call
		int gene;
		bool diagnosed; // 0 = Not hospitalized before, 1 = Hospitalized before
		double t;
		double t_conduct;
		double t_cum;
		double t_hospital;
		double t_queue;
		// Model input port
		std::vector<int> signal_from_bene;
		std::vector<int> signal_from_provider;
		// Model output port
		std::vector<int> signal_to_bene;
		std::vector<int> signal_to_provider;
		static std::list<Provider*> providers;

	private:

		double tahead;
		int total;
		int influence;
		double threshold;
		double progression;
		double risk_aversion;
};

#endif /* BENE_H_ */
