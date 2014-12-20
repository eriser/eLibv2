#include <Blocks/modScalerConnector.h>

using namespace eLibV2;

double ScalerConnector::processIOs()
{
	double input = 0.0;

	for (int inputIndex = CONNECTOR_INPUT1; inputIndex <= CONNECTOR_INPUT8; inputIndex++)
	{
		if (controlIOs.count(inputIndex) > 0)
			input += controlIOs[inputIndex]->processIOs();
	}

	ModuleLogger::print("%s::processIOs value: %lf %lf %lf -> %lf", getModuleName().c_str(), input, mFactor, mOffset, input * mFactor + mOffset);
	return input * mFactor + mOffset;
}
