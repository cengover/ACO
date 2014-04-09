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
int population = 3;
int connections = 1;
int numberOfProviders = 1;
double bene_signal_rate = 5.0;
double provider_service_rate = 10.0;
const double termination_time = 100.0;
const int capacity = 1;
const double w_health_for_threshold = 0.2;
const int factors_progression = 5;
const double weights_in_progression[factors_progression] = {0.2,0.2,0.2,0.2,0.2};
const string path = "/home/ozi/Desktop/Data/";

// Random number seed assignment
//std::string str = std::getenv("Seed");
int Seed = 0;//atoi(str.c_str());
static adevs::rv* rand_str_ptr = new adevs::rv(Seed);
adevs::rv& rand_strm = *rand_str_ptr;

// Methods to print out
char* create_file_name(string name){

	// Write a header describing the data fields
	stringstream sstm;
	sstm << path<< name<< population<<"_"<<connections<<"_"<<numberOfProviders<<"_"<<bene_signal_rate
				<<"_"<<provider_service_rate<<"_"<<Seed<<".txt";
	string file = sstm.str();
	char * output = (char *) file.c_str();
	return output;
}

void output_bene(BeneNetwork* beneN){

	char * bene_file = create_file_name("Bene_");
	ofstream bene_output;
	bene_output.open(bene_file);
	list<Bene*>::iterator bene = beneN->beneficiaries.begin();
		for (bene = beneN->beneficiaries.begin(); bene != beneN->beneficiaries.end(); bene++){

			bene_output<<"Bene "<<population<<" "<<connections<<" "<<numberOfProviders<<" "<<bene_signal_rate
					<<" "<<provider_service_rate<<" "<<Seed<<" "<<(*bene)->id<<" "<<(*bene)->health<<" "<<(*bene)->behavior<<" "<<(*bene)->diagnosed<<" "
			<<(*bene)->t_cum<< endl;
	}
	bene_output.close();
}

void output_provider(BeneNetwork* beneN){

	char * pro_file = create_file_name("Provider_");
	ofstream pro_output;
	pro_output.open(pro_file);
	list<Provider*>::iterator pro = beneN->providers.begin();
	for (pro = beneN->providers.begin(); pro != beneN->providers.end(); pro++){

		pro_output << "Provider "<<population<<" "<<connections<<" "<<numberOfProviders<<" "<<bene_signal_rate
			<<" "<<provider_service_rate<<" "<<Seed<<" "<<(*pro)->id<<" "<<(*pro)->busy_time<<" "<<(*pro)->total_patients<<" "<<(*pro)->distinct_patients
			<<" "<<(*pro)->service_cost<<" "<<(*pro)->intervention_budget<<endl;
	}
	pro_output.close();
}

// Exception Handling
int exception_handler(){

	int return_value = 1;
	try{
		if (connections > population-1){

			throw "the number of connections can not be greater than population size-1";
		}
		double t = 0;
		for (int i = 0; i <5;i++){
				t = t+weights_in_progression[i];

		}
		if (t!= 1.0){

			throw "the summation of weights must sum up to 1.0";
		}
	}
	catch (const char* msg){

		cerr << msg << endl;
		return_value = 0;
	}
	return return_value;
}

int main(){

	// Check for exceptions
	if (exception_handler() == 0){

		return 0;
	}
	// Measure the elapsed time
	clock_t begin = clock();

	// SCENARIOS
	//for(connections = 1; connections < 26; connections+=5){

		//for(provider_service_rate = 0.0;provider_service_rate<10.0;provider_service_rate+=2){

			//for(bene_signal_rate = 1.0;bene_signal_rate<10.0;bene_signal_rate+=2){

				//for (Seed = 1; Seed < 31;++Seed){

					// Create the model
					BeneNetwork* beneN = new BeneNetwork();
					// Create the simulator
					Simulator<IO>* sim = new Simulator<IO>(beneN);
					// Run the simulation
					while (sim->nextEventTime() <= termination_time){

						// Output next event time
						cout<<sim->nextEventTime()<<endl;
						sim->execNextEvent();
					}
					// Bene output for each scenario and replication
					output_bene(beneN);
					// Provider output for each scenario and replication
					output_provider(beneN);
					// Clean up and exit
					delete beneN;
					delete sim;
				//}
			//}
		//}
	//}

	clock_t end = clock();
	double elapsed_secs = double(end - begin)/ CLOCKS_PER_SEC;
	cout<<elapsed_secs<<endl;
	return 0;
}

