/*
 * main.cpp
 *
 *  Created on: Mar 3, 2014
 *      Author: ozi
 */
#include "BeneNetwork.h"
#include "Provider.h"
#include "Bene.h"
#include "Signal.h"
#include "config.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <stdlib.h>
using namespace adevs;
using namespace std;

//DEFINE PARAMETERS HERE
const int population = 5;
const int connections = 100;
const int numberOfProviders = 20;
const double bene_signal_rate = 3.0;
const double provider_service_rate = 0.001;
const double memory_count = 3;
const double influence_rate = 0.60;

//std::string str = std::getenv("Seed");
const unsigned int Seed = 12211;//atoi(str.c_str());
static adevs::rv* rand_str_ptr = new adevs::rv(Seed);
adevs::rv& rand_strm = *rand_str_ptr;

int main()
{
	// Create the model
	BeneNetwork* beneN = new BeneNetwork();
	// Create the simulator
	Simulator<IO>* sim = new Simulator<IO>(beneN);
	// Run the simulation
	while (sim->nextEventTime() <= 100.0)
	{
		// Output next event time
		// cout<<sim->nextEventTime()<<endl;
		sim->execNextEvent();
	}

	// Print out outcome measures at termination
	if (sim->nextEventTime() > 100){

		for (int i = 0; i< numberOfProviders; i++){

			cout << "Provider "<< i<< " total busy time is "<<beneN->providers[i]->busy_time<<endl;
			cout << "Provider "<< i<< " total patients who are served is "<<beneN->providers[i]->total_patients<<endl;
			cout << "Provider "<< i<< " distinct patients who are served is "<<beneN->providers[i]->distinct_patients<<endl;
		}

		cout << "Total number of visits is "<<beneN->observer[0]->total_number_of_patients<<endl;
	}

	// Clean up and exit
	delete beneN;
	delete sim;
	return 0;
}
