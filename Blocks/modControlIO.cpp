#include <Blocks/modControlIO.h>

#include <Util/modLogger.h>

using namespace eLibV2;

double ControlIO::processIOs()
{
	double res = 0.0;

	ModuleLogger::print("%p ControlIO: processing input %i", this, controlIOs.size());
	for (std::map<const int, ControlIO*>::iterator it = controlIOs.begin(); it != controlIOs.end(); it++)
	{
		res = it->second->processIOs();
	}
	return res;
}