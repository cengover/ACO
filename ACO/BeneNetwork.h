/*
 * BeneNetwork.h
 *
 *  Created on: Mar 3, 2014
 *      Author: ozi
 */

#ifndef BENENETWORK_H_
#define BENENETWORK_H_
#include "adevs.h"
#include "Bene.h"
#include "config.h"
#include "Provider.h"
#include "Payer.h"
#include <list>
#include <vector>

/**
 * This class implements Network as a Directed Graph
 */
class BeneNetwork: public adevs::Digraph<Signal*> {
	public:
		BeneNetwork();
		// Get the number of benes
		int getBeneCount();
		// This is for the bene set
		std::list<Bene*> beneficiaries;
		// This is for the bene set
		std::list<Provider*> providers;
		std::vector<Payer*> payer;
	private:
		// Method for adding a benes to the network
		void add_bene();
		// Method for adding a providers to the network
		void add_provider();

};

#endif /* BENENETWORK_H_ */
