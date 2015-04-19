#include <MIDI/modMidiEventHandler.h>

using namespace eLibV2::MIDI;

void MidiEventHandler::insertEvent(const int channel, const MidiEvent& event)
{
    if (channelInRange(channel))
        mMidiEvents[channel].push_back(event);
}

bool MidiEventHandler::deleteEvent(const int channel, const MidiEvent& event)
{
    bool deleted = false;
    int noteToSearch = event.getNote();

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

MidiEventVector MidiEventHandler::getEvents(const int channel) const
{
    MidiEventVector events;
    int size = 0;
    if (channelInRange(channel))
    {
        for (ConstMidiEventIterator it = mMidiEvents[channel].begin(); it != mMidiEvents[channel].end(); it++)
            events.push_back(*it);
        size = mMidiEvents[channel].size();
    }
    return events;
}

bool MidiEventHandler::hasEventsOnChannel(const int channel) const
{
    bool has = false;

    if (channelInRange(channel))
        has = (mMidiEvents[channel].size() != 0);

    return has;
}

bool MidiEventHandler::hasEventsOnAnyChannel() const
{
    bool hasEvents = false;

    for (int channelIndex = MIDI_CHANNEL_MIN; channelIndex < MIDI_CHANNEL_MAX; channelIndex++)
    {
        if (hasEventsOnChannel(channelIndex))
        {
            hasEvents = true;
            break;
        }
    }
    return hasEvents;
}
