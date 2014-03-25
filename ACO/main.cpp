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
const int population = 2;
const int connections = 1;
const int numberOfProviders = 1;
const double bene_signal_rate = 10.0;
const double provider_service_rate = 5.0;
const double influence_rate = 0.60;
const double termination_time = 100.0;

//std::string str = std::getenv("Seed");
const unsigned int Seed = 2;//atoi(str.c_str());
static adevs::rv* rand_str_ptr = new adevs::rv(Seed);
adevs::rv& rand_strm = *rand_str_ptr;

int main(){

	// Check for exceptions
	try{
		if (connections > population-1){

			throw "the number of connections can not be greater than population size -1";
		}
	}
	catch (const char* msg){

		cerr << msg << endl;
		return 0;
	}
	// Create the model
	BeneNetwork* beneN = new BeneNetwork();
	// Create the simulator
	Simulator<IO>* sim = new Simulator<IO>(beneN);
	// Run the simulation
	while (sim->nextEventTime() <= termination_time){

		// Output next event time
		cout<<sim->nextEventTime()<<endl;
		list<Provider*>::iterator pro = beneN->providers.begin();
		for (pro = beneN->providers.begin(); pro != beneN->providers.end(); pro++){

			cout<<"Provider "<<(*pro)->id<<" "<<(*pro)->busy_time<<" "<<(*pro)->distinct_patients<<" "<<(*pro)->t<< endl;
		}

		list<Bene*>::iterator bene = beneN->beneficiaries.begin();
		for (bene = beneN->beneficiaries.begin(); bene != beneN->beneficiaries.end(); bene++){

			cout<<"Bene "<<(*bene)->id<<" "<<(*bene)->behavior<<" "<<(*bene)->health<<" "<<(*bene)->hospitalized<<" "<<(*bene)->t<< endl;
		}

		sim->execNextEvent();
	}

	// Print out outcome measures at termination
	if (sim->nextEventTime() > 100){

		list<Provider*>::iterator pro = beneN->providers.begin();
		for (pro = beneN->providers.begin(); pro != beneN->providers.end(); pro++){

			cout << "Provider "<< (*pro)->id<< " total busy time is "<<(*pro)->busy_time<< endl;
			cout << "Provider "<< (*pro)->id<< " total patients who are served is "<<(*pro)->total_patients<<endl;
			cout << "Provider "<<(*pro)->id<< " distinct patients who are served is "<<(*pro)->distinct_patients<<endl;
		}
		cout << "Total number of visits is "<< beneN->payer[0]->total_number_of_patients<< endl;
	}

	// Clean up and exit
	delete beneN;
	delete sim;
	return 0;
}
