/*
* modConstantConnector.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODCONSTANTCONNECTOR_H_
#define MODCONSTANTCONNECTOR_H_

#include <Blocks/modInputConnector.h>

#include <Util/modLogger.h>

#include <vector>

namespace eLibV2
{
	class ConstantConnector : public InputConnector
	{
	public:
		ConstantConnector(double value) : BaseName("ConstantConnector"), mValue(value) {}

		virtual void putValue(double value) { ModuleLogger::print("putValue: %lf", value); mValue = value; }
		virtual double processIOs() { return processInputs(); }

		double processInputs() { ModuleLogger::print("%s::processInputs value: %lf", getModuleName().c_str(), mValue); return mValue; }

	protected:
		double mValue;
	};
}

#endif
