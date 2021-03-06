/*
 * config.h
 *
 *  Created on: Mar 3, 2014
 *      Author: ozi
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "adevs.h"
#include "Signal.h"

// Parameters that are used for parameter sweep by the script
extern adevs::rv& rand_strm;
extern int Seed;
extern int population;
extern int connections;
extern int numberOfProviders;
extern double bene_signal_rate;
extern double provider_service_rate;
extern double medication_period;
extern double intervention_rate;
extern double max_threshold;

// Constants
extern const double termination_time;
extern const double w_health_for_threshold;
extern const int factors_progression;
extern const double weights_in_progression[];
extern const string path;

#endif /* CONFIG_H_ */
