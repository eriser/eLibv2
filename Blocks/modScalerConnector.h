/*
* modScalerConnector.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODSCALERCONNECTOR_H_
#define MODSCALERCONNECTOR_H_

#include <Blocks/modBaseConnector.h>

namespace eLibV2
{
	class ScalerConnector : public BaseConnector
	{
	public:
		ScalerConnector(double factor, double offset) : BaseName("ScalerConnector"), mFactor(factor), mOffset(offset) {}
		ScalerConnector(std::string name, double factor, double offset) : BaseName(name), mFactor(factor), mOffset(offset) {}

		virtual void Init() {}
		virtual double processIOs();

	protected:
		double mFactor, mOffset;
	};
}

#endif
