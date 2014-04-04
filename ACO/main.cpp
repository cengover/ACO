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
#include <fstream>
#include <string.h>
#include <cstring>
#include <cstdio>
#include <sstream>

using namespace adevs;
using namespace std;

//DEFINE PARAMETERS HERE
int population = 100;
int connections = 5;
int numberOfProviders = 1;
double bene_signal_rate = 1.0;
double provider_service_rate = 0.5;
const double termination_time = 100.0;

//std::string str = std::getenv("Seed");
int Seed = 0;//atoi(str.c_str());
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
	// SCENARIOS
	ofstream myfile1;

	stringstream sstm2;
	sstm2 << "Provider_"<<".txt";
	string ans1 = sstm2.str();
	char * ans3 = (char *) ans1.c_str();
	myfile1.open(ans3);
	for(connections = 5; connections < 26; connections+=10){

		for(provider_service_rate = 0.5;provider_service_rate<5;provider_service_rate+=2){

			for(bene_signal_rate = 1.0;bene_signal_rate<12;bene_signal_rate+=5){

				for (Seed = 0; Seed < 30;++Seed){

				// Create the model
					BeneNetwork* beneN = new BeneNetwork();
					// Create the simulator
					Simulator<IO>* sim = new Simulator<IO>(beneN);
					// Run the simulation
					while (sim->nextEventTime() <= termination_time){

						// Output next event time
						//cout<<sim->nextEventTime()<<endl;
						sim->execNextEvent();
					}
					stringstream sstm;
					sstm << "Bene_"<<population<<"_"<<connections<<"_"<<numberOfProviders<<"_"<<bene_signal_rate
							<<"_"<<provider_service_rate<<"_"<<Seed<<".txt";
					string ans = sstm.str();
					char * ans2 = (char *) ans.c_str();

					ofstream myfile;
					myfile.open(ans2);

					// Write a header describing the data fields
					list<Bene*>::iterator bene = beneN->beneficiaries.begin();
					for (bene = beneN->beneficiaries.begin(); bene != beneN->beneficiaries.end(); bene++){

						myfile<<"Bene "<<(*bene)->id<<" "<<(*bene)->health<<" "<<(*bene)->behavior<<" "<<(*bene)->diagnosed<<" "<<(*bene)->t_cum<< endl;
					}
					myfile.close();

					list<Provider*>::iterator pro = beneN->providers.begin();
					for (pro = beneN->providers.begin(); pro != beneN->providers.end(); pro++){

						cout << "Provider "<<population<<" "<<connections<<" "<<numberOfProviders<<" "<<bene_signal_rate
								<<" "<<provider_service_rate<<" "<<Seed<<" "<<(*pro)->id<<" "<<(*pro)->busy_time<<" "<<(*pro)->total_patients<<" "<<(*pro)->distinct_patients
								<<" "<<(*pro)->service_cost<<" "<<(*pro)->intervention_budget<<endl;
					}
					// Clean up and exit
					delete beneN;
					delete sim;
				}
			}
		}

	}
	myfile1.close();
	return 0;
}
