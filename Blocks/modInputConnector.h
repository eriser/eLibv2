#ifndef MODINPUTCONNECTOR_H_
#define MODINPUTCONNECTOR_H_

#include <Base/modBaseConnection.h>

#include <Util/modLogger.h>

#include <vector>

namespace eLibV2
{
	class InputConnector : public BaseConnection
	{
	public:
		InputConnector(std::string name = "InputConnector")	: BaseName(name) {}

		virtual void setInput(double value) { ModuleLogger::print("%s::setValue: %lf", getModuleName().c_str(), value); mValue = value; }
		virtual double processConnection() { return processInputs(); }
		double processInputs() { ModuleLogger::print("%s::processInputs value: %lf", getModuleName().c_str(), mValue); return mValue; }

	protected:
		double mValue;
	};
}

#endif
