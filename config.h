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

// Parameters that are used in main() method
extern adevs::rv& rand_strm;
extern int Seed;
extern int population;
extern int connections;
extern int numberOfProviders;
extern const double w_health_for_threshold;
extern const int factors_progression;
extern const double weights_in_progression[];
extern const int capacity;
extern const bool par;
extern double bene_signal_rate;
extern double provider_service_rate;
extern const double termination_time;
extern const double t_lookahead;
extern const string path;

#endif /* CONFIG_H_ */
