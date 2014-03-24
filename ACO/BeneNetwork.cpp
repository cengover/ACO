/*
 * BeneNetwork.cpp
 *
 *  Created on: Mar 3, 2014
 *      Author: ozi
 */
#include "Bene.h"
#include "BeneNetwork.h"
#include "Provider.h"
#include "Observer.h"

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
	Observer* obsrv = new Observer();
	add(obsrv);
	observer.push_back(obsrv);

	// Connect Digraph here
	for (int i = 0; i< population; i++){

		Bene* src = beneficiaries[i];
		Bene* tgt = new Bene();
		for (int j = 0; j < connections; j++)
		{
			int inx;
			inx = get_member(population);
			while (inx == i){
				inx = get_member(population);
			}
			tgt = beneficiaries[inx];
			couple(src,src->signal_out[0],tgt,tgt->signal_in[0]);
		}
		int provider = get_member(numberOfProviders);
		couple(src,src->signal_out[1],providers[provider],providers[provider]->signal_in);
		couple(providers[provider],providers[provider]->signal_out,src,src->signal_in[1]);
	}
	// Connect Providers and Observer
	for (int i = 0; i< numberOfProviders; i++){

		couple(providers[i],providers[i]->observer_out, obsrv, obsrv->output_port);
	}
}

// Add Beneficiaries
void BeneNetwork::add_bene() {

	for (int i = 0; i < population;i++){

		Bene* bene = new Bene();
		add(bene);
		bene->id = i;
		beneficiaries.push_back(bene);
	}
}

// Add Providers
void BeneNetwork::add_provider() {

	for (int i = 0; i < numberOfProviders;i++){

		Provider* pro = new Provider();
		add(pro);
		pro->id = i;
		providers.push_back(pro);
	}
}
