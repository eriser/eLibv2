#include <Blocks/modControlIO.h>

using namespace eLibV2;

double ControlIO::processIOs()
{
	double res = 0.0;

	ModuleLogger::print("%s::processIO (inputs: %i)", getModuleName().c_str(), controlIOs.size());
	for (controlIOiterator it = controlIOs.begin(); it != controlIOs.end(); it++)
	{
		res = it->second->processIOs();
	}
	return res;
}

void ControlIO::testChaining()
{
	if (controlIOs.size() > 0)
	{
		ModuleLogger::print("%s connects to", getModuleName().c_str());
		for (controlIOiterator it = controlIOs.begin(); it != controlIOs.end(); it++)
			it->second->testChaining();
	}
	else
		ModuleLogger::print("%s", getModuleName().c_str());
}