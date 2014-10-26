#include <Base/modAudioIO.h>
#include <Util/modLogger.h>

using namespace eLibV2;

double AudioIO::processAudioInputs()
{
	double res = 0.0;

	ModuleLogger::setActive(true);
	for (int i = 0; i < audioInputs.size(); i++)
	{
		if (audioInputs[i])
		{
//			ModuleLogger::print("%p AudioIO: processing input %i %p", this, i, audioInputs[i]);
			res = audioInputs[i]->processAudioInputs();
		}
	}
	return res;
}