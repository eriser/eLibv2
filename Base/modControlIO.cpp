#include <Base/modControlIO.h>
#include <Util/modLogger.h>

using namespace eLibV2;

double ControlIO::processControlInputs()
{
	double res = 0.0;

	ModuleLogger::setActive(true);
//	ModuleLogger::print("%p ControlIO: processing input", this);
	for (int i = 0; i < controlInputs.size(); i++)
	{
		if (controlInputs[i])
		{
			res = controlInputs[i]->processControlInputs();
		}
	}
	return res;
}