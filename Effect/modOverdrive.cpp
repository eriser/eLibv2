#include <Effect/modOverdrive.h>

using namespace eLibV2;

double FxOverdrive::Process(double input)
{
    double ret = input * dLevel;
	ret = clamp(ret, -1.0, 1.0);

	return ret;
}

double FxOverdrive::processIOs()
{
	double input = 0.0;

	if (isAttached(OVERDRIVE_LEVEL))
		setLevel(controlIOs[OVERDRIVE_LEVEL]->processIOs());
	if (isAttached(OVERDRIVE_INPUT))
		input = controlIOs[OVERDRIVE_INPUT]->processIOs();
	ModuleLogger::print("%s::process %lf", getModuleName().c_str(), input);

	return Process(input);
}
