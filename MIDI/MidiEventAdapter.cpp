#include <MIDI/MidiEventAdapter.h>

using namespace eLibV2::MIDI;

double MidiEventAdapter::processConnection(void)
{
    double res = 0.0;
    MidiEventVector events;

    if (mHandler)
    {
        if (mHandler->hasEventsOnChannel(mChannel))
        {
            if (mHandler->getEvents(mChannel, events))
                res = events[0].getNote();
        }
    }
    return res;
}

