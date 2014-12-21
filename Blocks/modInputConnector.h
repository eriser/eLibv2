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
		InputConnector() : BaseName("InputConnector") {}
		InputConnector(std::string name) : BaseName(name) {}

		virtual void setValue(double value) { ModuleLogger::print("%s::setValue: %lf", getModuleName().c_str(), value); mValue = value; }
		virtual double processIOs() { return processInputs(); }

		double processInputs() { ModuleLogger::print("%s::processInputs value: %lf", getModuleName().c_str(), mValue); return mValue; }

	protected:
		double mValue;
	};
}

#endif
