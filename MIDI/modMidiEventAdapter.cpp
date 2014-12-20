#include <MIDI/modMidiEventAdapter.h>

using namespace eLibV2;

double MidiEventAdapter::processIOs()
{
	double res = 0.0;
	std::vector<MidiEvent> events;

	if (mHandler)
	{
		if (mHandler->hasEventsOnChannel(mChannel))
		{
			mHandler->getEvents(mChannel, events);
			res = events[0].getNote();
		}
	}
	return res;
}

