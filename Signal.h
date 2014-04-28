/*
 * Signal.h
 *
 *  Created on: Mar 11, 2014
 *      Author: ozi
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

/**
Signal that is used as a PortValue
*/
struct Signal
{
	int id;
	int lifestyle;
	int health;
	int insurance;
	int intervention;
	bool diagnosed;
	double entry_time;
	double service_duration;
	double t_queue;
	int from_bene;
	int from_provider;
	int to_provider;
	int to_payer;
};

/// Create an abbreviation for the Clerk's input/output type.
typedef adevs::PortValue<Signal*> IO;

#endif /* SIGNAL_H_ */
