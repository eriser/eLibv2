#include <Blocks/modControlIO.h>

double ControlIO::processIOs()
{
	double res = 0.0;

	ModuleLogger::print("%s::processIO (inputs: %i)", getModuleName().c_str(), controlIOs.size());
	for (controlIOiterator it = controlIOs.begin(); it != controlIOs.end(); it++)
		res = it->second->processIOs();
	return res;
}

void ControlIO::testChaining(int depth)
{
	char temp[1024];

	depth++;
	memset(temp, 0, sizeof(temp));
	for (int i = 1; i < depth; i++)
		sprintf(temp, " %s", temp);
	sprintf(temp, "%s%s", temp, getModuleName().c_str());
	ModuleLogger::print(temp);

	if (controlIOs.size() > 0)
	{
		for (controlIOiterator it = controlIOs.begin(); it != controlIOs.end(); it++)
			it->second->testChaining(depth);
	}
}