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
		/// Update Duration
		void update_duration();
		/// Create signal for bene to bene interaction
		Signal* create_signal_to_bene();
		/// Create signal for bene to provider interaction
		Signal* create_signal_to_provider();
		int id; // Identifier of bene
		int insurance; // 0 = Exist, 1 = Not-exist
		int lifestyle; // 0 = Un-Healthy, 1 = Healthy
		int health; // 0 = Healthy, 1 = Pre-Diabetes, 2 = Diabetes, 3 = Insulin Dependence, 4 = Death (Is not emplemented yet)
		int hospitalized; // 0 = Not Hospitalized, 1 = At Hospital
		int intervention;// 0 = Not received follow up call, 1 = Received phone from hospital
		int gene; // 0 = No genetic vulnerability, 1 = Vulnerable to diabetes
		int memory; // How many realizations (internal transition function calls) a bene will keep track of influences
		int memory_count; // The count of the number of realizations.
		double memory_factor; // Importance given to recent influence
		double tendency; // Tendency to switch lifestyle
		bool diagnosed; // 0 = Not hospitalized before, 1 = Hospitalized before
		double duration; // Intensity of unhealthy lifestyle for each time tick spent out of provider (between 0 and 1)
		double t; // Synchronized with Simulation Time
		double t_conduct; // Time stamp at which a bene starts conducting unhealthy lifestyle
		double t_cum; // Cumulative amount of time in unhealthy lifestyle
		double t_hospital; // Time spent at provider
		double t_queue; // Time spent in the queue at provider
		// Model input port
		std::vector<int> signal_from_bene;
		std::vector<int> signal_from_provider;
		// Model output port
		std::vector<int> signal_to_bene;
		std::vector<int> signal_to_provider;
		// List of providers
		static std::list<Provider*> providers;

	private:

		double tahead; // Time to next event
		int total; // Total number of influences
		int influence; // The number of unhealthy lifestyle signals
		double threshold; // Threshold of progress of diabetes
		double progression; // Progress of diabetes (between 0 and 1)
		double risk_aversion; // To be used for un-insured benes
};

#endif /* BENE_H_ */
