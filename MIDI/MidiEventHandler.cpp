#include <MIDI/MidiEventHandler.h>

using namespace eLibV2::MIDI;

void MidiEventHandler::insertEvent(const SInt16 channel, const MidiEvent& event)
{
    if (channelInRange(channel))
        mMidiEvents[channel].push_back(event);
}

bool MidiEventHandler::deleteEvent(const SInt16 channel, const MidiEvent& event)
{
    bool deleted = false;
    SInt16 noteToSearch = event.getNote();

    if (channelInRange(channel))
    {
        for (MidiEventIterator eventIterator = mMidiEvents[channel].begin(); eventIterator != mMidiEvents[channel].end(); eventIterator++)
        {
            if (eventIterator->getNote() == noteToSearch)
            {
                // adjust iterator to the element after the deleted one
                eventIterator = mMidiEvents[channel].erase(eventIterator);
                deleted = true;
                break;
            }
        }
    }
    return deleted;
}

bool MidiEventHandler::getEvents(const SInt16 channel, MidiEventVector& events) const
{
    bool res = false;
    if (channelInRange(channel))
    {
        events = mMidiEvents[channel];
        if (events.size() > 0)
            res = true;
#if 0
        // BEWARE: race condition
        if (mMidiEvents[channel].size())
        {
            for (ConstMidiEventIterator it = mMidiEvents[channel].begin(); it != mMidiEvents[channel].end(); it++)
                events.push_back(*it);
        }
#endif
    }
    return res;
}

bool MidiEventHandler::hasEventsOnChannel(const SInt16 channel) const
{
    bool has = false;

    if (channelInRange(channel))
        has = (mMidiEvents[channel].size() != 0);

    return has;
}

bool MidiEventHandler::hasEventsOnAnyChannel() const
{
    bool hasEvents = false;

    for (SInt16 channelIndex = MIDI_CHANNEL_MIN; channelIndex < MIDI_CHANNEL_MAX; channelIndex++)
    {
        if (hasEventsOnChannel(channelIndex))
        {
            hasEvents = true;
            break;
        }
    }
    return hasEvents;
}
