#include <MIDI/MidiEventAdapter.h>

using namespace eLibV2::MIDI;

double MidiEventAdapter::processConnection()
{
    double res = 0.0;
    MidiEventVector events;

    if (mHandler)
    {
        if (mHandler->hasEventsOnChannel(mChannel))
        {
            events = mHandler->getEvents(mChannel);
            if (events.size())
                res = events[0].getNote();
        }
    }
    return res;
}

