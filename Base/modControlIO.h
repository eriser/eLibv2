/*
* modControlIO.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODCONTROLIO_H_
#define MODCONTROLIO_H_

#include <vector>

namespace eLibV2
{
	class ControlIO
	{
	public:
		// polymophism doesn't work without virtual!!!
		virtual void connectControlInput(ControlIO* input) { controlInputs.push_back(input); }
		virtual double processControlInputs();

	protected:
		std::vector<ControlIO*> controlInputs;
	};
}

#endif
