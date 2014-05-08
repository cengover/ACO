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
		// This is for the bene set
		std::list<Bene*> beneficiaries;
		// This is for the provider set
		std::list<Provider*> providers;
		std::vector<Payer*> payer;
		/// Lookahead
		double lookahead(){

			return t_lookahead;
		}
		/// Begin lookahead
		void beginLookahead(){

			/*chkpt.beneficiaries = this->beneficiaries;
			chkpt.providers = this->providers;
			chkpt.payer = this->payer;*/
		}
		/// End lookahead
		void endLookahead(){

			/*this->beneficiaries = chkpt.beneficiaries;
			this->providers = chkpt.providers;
			this->payer = chkpt.payer;*/
		}
	private:
		// Method for adding a benes to the network
		void add_bene();
		// Method for adding a providers to the network
		void add_provider();
		// Method for connecting DiGraph' s all components
		void random_connect();
		// Method for connecting benes
		void connect_providers(Bene* src);

		struct checkpoint{
			/*// This is for the bene set
			std::list<Bene*> beneficiaries;
			// This is for the provider set
			std::list<Provider*> providers;
			std::vector<Payer*> payer;*/

		};
		checkpoint chkpt;
};

#endif /* BENENETWORK_H_ */
