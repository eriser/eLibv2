#include <Blocks/modBaseConnector.h>

using namespace eLibV2;

double BaseConnector::processConnection()
{
	double res = 0.0;

	ModuleLogger::print("%s::processIO (inputs: %i)", getModuleName().c_str(), connections.size());
	for (controlIOiterator it = connections.begin(); it != connections.end(); it++)
		res = it->second->processConnection();
	return res;
}

void BaseConnector::printConnection(int depth)
{
	std::stringstream oss;

	depth++;
	for (int i = 1; i < depth; i++)
		oss << ' ';
	oss << getModuleName().c_str() << " " << (int)getNumberOfInputs() << "/" << (int)getNumberOfOutputs();
	ModuleLogger::print(oss.str().c_str());

	if (connections.size() > 0)
	{
		for (controlIOiterator it = connections.begin(); it != connections.end(); it++)
			it->second->printConnection(depth);
	}
}