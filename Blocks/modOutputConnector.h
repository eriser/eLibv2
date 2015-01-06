#ifndef MODOUTPUTCONNECTOR_H_
#define MODOUTPUTCONNECTOR_H_

#include <Blocks/modBaseConnector.h>

#include <Util/modLogger.h>

#include <vector>

namespace eLibV2
{
	class OutputConnector : public BaseConnector
	{
	public:
		OutputConnector(std::string name = "InputConnector") : BaseName(name), BaseConnector(1, 0) {}

	protected:
		double mValue;
	};
}

#endif
