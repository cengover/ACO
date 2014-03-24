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
	int state;
	int health_status;
	int id;
	bool serviced_before;
	double service_time;
};

/// Create an abbreviation for the Clerk's input/output type.
typedef adevs::PortValue<Signal*> IO;

#endif /* SIGNAL_H_ */
