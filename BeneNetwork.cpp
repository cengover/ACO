/*
 * BeneNetwork.cpp
 *
 *  Created on: Mar 3, 2014
 *      Author: ozi
 */
#include "Bene.h"
#include "BeneNetwork.h"
#include "Provider.h"
#include "Payer.h"
#include <iterator>

using namespace adevs;
using namespace std;

BeneNetwork::BeneNetwork():Digraph<Signal*>() // call the parent constructor
{
	// Add the initial objects to the network - Providers and Beneficiaries
	add_provider();
	add_bene();
	random_connect();
}

// Get Population member' s index
static int get_member(int max){

	int t = ceil(rand_strm.uniform(0.0,max-1)-0.5);
	return t;
}

// Add Beneficiaries
void BeneNetwork::add_bene() {

	for (int i = 0; i < population;i++){

		Bene* bene = new Bene();
		bene->id = i;
		add(bene);
		beneficiaries.push_back(bene);
	}
}

// Add Providers
void BeneNetwork::add_provider() {

	for (int i = 0; i < numberOfProviders;i++){

		Provider* pro = new Provider();
		pro->id = population+i;
		add(pro);
		pro->setProc(0);
		providers.push_back(pro);
	}
}

//Randomly connect all Digraph components
void BeneNetwork::random_connect(){

	// Connect Digraph here
	list<Bene*>::iterator src = beneficiaries.begin();
	list<Bene*>::iterator tgt = beneficiaries.begin();
	list<Provider*>::iterator prit = providers.begin();
	std::vector<int> ids(connections);

	for (src = beneficiaries.begin(); src != beneficiaries.end(); src++){


		std::fill (ids.begin(),ids.end(),-1);
		bool check = false;

		// Randomly connect Benes based on the number of connections
		for (int j = 0; j < connections; j++){

			int inx = get_member(population);
			ids[j] = inx;
			tgt = beneficiaries.begin();
			std::advance(tgt,inx);
			for (int n = 0; n < j; n++){

				if(ids[n] == inx){

					check = true;
					break;
				}
			}
			if((*src)->id == (*tgt)->id){

				check = true;
			}

			while (check == true){

				check = false;
				inx = get_member(population);
				ids[j] = inx;
				tgt = beneficiaries.begin();
				std::advance(tgt,inx);
				for (int n = 0; n < j; n++){

					if(ids[n] == inx){

						check = true;
						break;
					}

				}
				if((*src)->id == (*tgt)->id){

					check = true;
				}
			}
			// Create ports, in other words links
			(*src)->signal_to_bene.push_back((*tgt)->id);
			(*tgt)->signal_from_bene.push_back((*src)->id);

			int s  = (*src)->signal_to_bene.size()-1;
			int t  = (*tgt)->signal_from_bene.size()-1;

			couple((*src),(*src)->signal_to_bene[s],(*tgt),(*tgt)->signal_from_bene[t]);
		}
		// Connect providers and other components here
		connect_providers((*src));
	}
}

/// Connect benes to providers
void BeneNetwork::connect_providers(Bene* src){

	// Connect Providers and Beneficiaries here!
	list<Provider*>::iterator prit = providers.begin();
	int provider = get_member(numberOfProviders);
	std::advance(prit,provider);

	// Create links
	src->signal_to_provider.push_back((*prit)->id);
	src->signal_from_provider.push_back((*prit)->id);
	(*prit)->signal_in.push_back(src->id);
	(*prit)->signal_out.push_back(src->id);
	int s  = src->signal_to_provider.size()-1;
	int t  = (*prit)->signal_in.size()-1;

	couple(src,src->signal_to_provider[s],(*prit),(*prit)->signal_in[t]);
	couple((*prit),(*prit)->signal_out[t],src,src->signal_from_provider[s]);

	// Connect Providers and Observer
	Payer* obsrv = new Payer();
	add(obsrv);
	payer.push_back(obsrv);
	for (prit = providers.begin(); prit != providers.end(); prit++){
		couple((*prit),(*prit)->payer_out, obsrv, obsrv->output_port);
	}
}
