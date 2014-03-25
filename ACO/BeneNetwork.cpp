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

// Get Population member' s index
static int get_member(int max){

	int t = ceil(rand_strm.uniform(0.0,max-1)-0.5);
	return t;
}

BeneNetwork::BeneNetwork():Digraph<Signal*>() // call the parent constructor
{
	// Add the initial objects to the network - Providers and Beneficiaries
	add_provider();
	add_bene();
	Payer* obsrv = new Payer();
	add(obsrv);
	payer.push_back(obsrv);

	// Connect Digraph here
	list<Bene*>::iterator src = beneficiaries.begin();
	list<Bene*>::iterator tgt = beneficiaries.begin();
	list<Provider*>::iterator prit = providers.begin();
	std::vector<int> ids(connections);

	for (src = beneficiaries.begin(); src != beneficiaries.end(); src++){


		std::fill (ids.begin(),ids.end(),-1);
		bool check = false;

		// Connect Benes
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

			couple((*src),(*src)->signal_out[0],(*tgt),(*tgt)->signal_in[0]);
		}
			// Connect Providers and Beneficiaries here!
			int provider = get_member(numberOfProviders);
			prit = providers.begin();
			std::advance(prit,provider);

			couple((*src),(*src)->signal_out[1],(*prit),(*prit)->signal_in);
			couple((*prit),(*prit)->signal_out,(*src),(*src)->signal_in[1]);
	}
	// Connect Providers and Observer
	for (prit = providers.begin(); prit != providers.end(); prit++){

		couple((*prit),(*prit)->observer_out, obsrv, obsrv->output_port);
	}
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
		pro->id = i;
		add(pro);
		providers.push_back(pro);
	}
}
