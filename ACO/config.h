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
extern const int population;
extern const int connections;
extern const int numberOfProviders;
extern const double bene_signal_rate;
extern const double provider_service_rate;
extern const double influence_rate;
extern const double termination_time;

#endif /* CONFIG_H_ */
