/*
* modInputConnector.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODINPUTCONNECTOR_H_
#define MODINPUTCONNECTOR_H_

#include <Blocks/modBaseConnector.h>

#include <Util/modLogger.h>

#include <vector>

namespace eLibV2
{
	class InputConnector: public BaseConnector
	{
	public:
		virtual void putValue(double value) { ModuleLogger::print("%lf", value); mValue = value; }
		virtual double processIOs() { return processInputs(); }

		double processInputs() { ModuleLogger::print("%p inputconnector: value: %lf", this, mValue); return mValue; }

	protected:
		double mValue;
	};
}

#endif
