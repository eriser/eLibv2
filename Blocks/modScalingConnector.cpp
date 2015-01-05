#include <Blocks/modScalingConnector.h>

using namespace eLibV2;

double ScalingConnector::processConnection()
{
	double input = 0.0;

	for (controlIOiterator it = connections.begin(); it != connections.end(); it++)
	{
		double value = it->second->processConnection();
		if (it->first == SCALING_FACTOR)
			mFactor = value;
		else if (it->first == SCALING_OFFSET)
			mOffset = value;
		else
			input += value;
	}

	ModuleLogger::print("%s::processIOs value: %lf %lf %lf -> %lf", getModuleName().c_str(), input, mFactor, mOffset, input * mFactor + mOffset);
	return input * mFactor + mOffset;
}
