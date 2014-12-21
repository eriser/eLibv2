#include <Blocks/modScalingConnector.h>

double ScalingConnector::processIOs()
{
	double input = 0.0;

	for (controlIOiterator it = controlIOs.begin(); it != controlIOs.end(); it++)
	{
		if (it->first == SCALING_FACTOR)
			mFactor = it->second->processIOs();
		else if (it->first == SCALING_OFFSET)
			mOffset = it->second->processIOs();
		else
			input += it->second->processIOs();
	}

	ModuleLogger::print("%s::processIOs value: %lf %lf %lf -> %lf", getModuleName().c_str(), input, mFactor, mOffset, input * mFactor + mOffset);
	return input * mFactor + mOffset;
}
