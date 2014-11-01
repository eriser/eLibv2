#include <MIDI/modMidiEventAdapter.h>

using namespace eLibV2;

MidiEventAdapter::MidiEventAdapter(MidiEventHandler *handler, int channel)
{
	mHandler = handler;
	mChannel = channel;
}

double MidiEventAdapter::processIOs()
{
	double res = 0.0;
	std::vector<MidiEvent> events;

	if (mHandler)
	{
		if (mHandler->hasEventsOnChannel(mChannel))
		{
			mHandler->getEvents(mChannel, events);
			res = events[mChannel].getNote();
		}
	}
	return res;
}

