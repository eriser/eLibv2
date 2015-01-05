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
		ConstantConnector(double value) : InputConnector("ConstantConnector"), mValue(value) {}

		virtual double processIOs() { return processInputs(); }
		double processInputs() { ModuleLogger::print("%s::processInputs value: %lf", getModuleName().c_str(), mValue); return mValue; }

	protected:
		const double mValue;
	};
}

#endif
