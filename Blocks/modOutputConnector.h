#ifndef MODOUTPUTCONNECTOR_H_
#define MODOUTPUTCONNECTOR_H_

#include <Base/modBaseConnection.h>

#include <Util/modLogger.h>

#include <vector>

namespace eLibV2
{
	class OutputConnector : public BaseConnection
	{
	public:
		OutputConnector(std::string name = "InputConnector") : BaseName(name) {}

	protected:
		double mValue;
	};
}

#endif
