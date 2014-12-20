/*
* modBaseConnector.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODBASECONNECTOR_H_
#define MODBASECONNECTOR_H_

#include <Base/modBaseModule.h>

#include <vector>

namespace eLibV2
{
	class BaseConnector : public BaseModule
	{
	public:
		BaseConnector() : BaseName("BaseConnector") {}
		BaseConnector(std::string name) : BaseName(name) {}

		void Init() {}
	};
}

#endif
