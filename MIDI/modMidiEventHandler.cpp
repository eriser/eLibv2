#include <MIDI/modMidiEventHandler.h>

using namespace eLibV2::MIDI;

MidiEventHandler::~MidiEventHandler(void)
{
    for (int i = 0; i < MAX_MIDI_CHANNELS; i++)
    {
        if (mMidiEvents[i])
            delete mMidiEvents[i];
    }
}

void MidiEventHandler::insertEvent(int channel, MidiEvent event)
{
    if (mMidiEvents[channel])
        mMidiEvents[channel]->push_back(event);
}

bool MidiEventHandler::deleteEvent(int channel, MidiEvent event)
{
    bool deleted = false;

    if (mMidiEvents[channel])
    {
        int noteToSearch = event.getNote();
        for (std::vector<MidiEvent>::iterator it = mMidiEvents[channel]->begin(); it != mMidiEvents[channel]->end(); it++)
        {
            if (it->getNote() == noteToSearch)
            {
                // adjust iterator to the element after the deleted one
                it = mMidiEvents[channel]->erase(it);
                deleted = true;
            }
        }
    }
    return deleted;
}

MidiEventVector MidiEventHandler::getEvents(int channel)
{
    MidiEventVector events;
    int size = 0;
    if (mMidiEvents[channel])
    {
        for (MidiEventIterator it = mMidiEvents[channel]->begin(); it != mMidiEvents[channel]->end(); it++)
            events.push_back(*it);
        size = mMidiEvents[channel]->size();
    }
    return events;
}

bool MidiEventHandler::hasEventsOnChannel(int channel)
{
    bool has = false;

    if (mMidiEvents[channel])
        has = (mMidiEvents[channel]->size() != 0);

    return has;
}

bool MidiEventHandler::hasEventsOnAnyChannel()
{
    bool hasEvents = false;

    for (int channelIndex = MidiEvent::MIDI_CHANNEL_1; channelIndex < MidiEvent::MIDI_CHANNEL_MAX; channelIndex++)
    {
        if (hasEventsOnChannel(channelIndex))
        {
            hasEvents = true;
            break;
        }
    }
    return hasEvents;
}

void MidiEventHandler::Init()
{
    for (int i = 0; i < MAX_MIDI_CHANNELS; i++)
        mMidiEvents[i] = new MidiEventVector();
}
