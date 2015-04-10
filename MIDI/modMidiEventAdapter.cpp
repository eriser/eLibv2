#include <MIDI/modMidiEventAdapter.h>

double MidiEventAdapter::processConnection()
{
    double res = 0.0;
    MidiEventVector events;

    if (mHandler)
    {
        if (mHandler->hasEventsOnChannel(mChannel))
        {
            events = mHandler->getEvents(mChannel);
            res = events[0].getNote();
        }
    }
    return res;
}

