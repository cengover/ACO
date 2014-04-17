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
#include "clock.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <cstring>
#include <cstdio>
#include <sstream>
#include "sys/types.h"
#include "sys/sysinfo.h"

using namespace adevs;
using namespace std;

//DEFINE PARAMETERS HERE
// Output path
const string path = "/home/ozi/Desktop/Data/";
const double termination_time = 200.0;
const bool par = false;
int population = 200;
int numberOfProviders = 1;


 ///*
int Seed = 1;
int connections = 2;
double bene_signal_rate = 5.0;
double provider_service_rate = 1.0;
 //*/

const double t_lookahead = 5;
const int capacity = 1; // To be changed after adding multiple servers/doctors
const double w_health_for_threshold = 0.2;
const int factors_progression = 5;
const double weights_in_progression[factors_progression] = {0.2,0.2,0.2,0.2,0.2};

// Environmental variables to use in scenarios (in run-abm.sh)
 /*
std::string str = std::getenv("Seed");
int Seed = atoi(str.c_str());
std::string str1 = std::getenv("connections");
int connections = atoi(str1.c_str());
std::string str2 = std::getenv("bene_signal_rate");
double bene_signal_rate = atoi(str2.c_str());
std::string str3 = std::getenv("provider_service_rate");
double provider_service_rate = atoi(str3.c_str());
 */
// Assign Ransom Number Seed
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
					<<" "<<provider_service_rate<<" "<<Seed<<" "<<(*bene)->id<<" "<<(*bene)->health<<" "<<(*bene)->behavior<<
					" "<<(*bene)->diagnosed<<" "<<(*bene)->t_cum<<" "<<" "<<(*bene)->t_queue<<" "<<(*bene)->t_hospital<<endl;
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

	// Record the start of elapsed time
	double cpu_begin = get_cpu_time();
	double wall_begin = get_wall_time();
	// Create the model
	BeneNetwork* beneN = new BeneNetwork();
	// Create the simulator
	ParSimulator<IO>* sim = new ParSimulator<IO>(beneN);
	//Simulator<IO>* sim = new Simulator<IO>(beneN);

	// Run the simulation
	// Output next event time
	//cout<<sim->nextEventTime()<<endl;
	sim->execUntil(termination_time);

	// Bene output for each scenario and replication
	output_bene(beneN);
	// Provider output for each scenario and replication
	output_provider(beneN);
	// Clean up and exit
	delete beneN;
	delete sim;
	// Record the end of elapsed time
	double cpu_end = get_cpu_time();
	double wall_end = get_wall_time();
	cout << "Real Time = " << wall_end - wall_begin << endl;
	cout << "CPU Time  = " << cpu_end  - cpu_begin  << endl;
	return 0;
}

