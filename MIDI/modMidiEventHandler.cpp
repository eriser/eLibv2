/*
 * modMidiEventHandler.cpp
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#include <MIDI/modMidiEventHandler.h>
#include <Util/modLogger.h>

using namespace eLibV2;

MidiEventHandler::MidiEventHandler(void)
{
    Init();
}

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
                // !!! iterator gets invalid when deleting or inserting elements !!!
                mMidiEvents[channel]->erase(it);
                deleted = true;
                break;
            }
        }
    }
    return deleted;
}

int MidiEventHandler::getEvents(int channel, std::vector<MidiEvent>& events)
{
    int size;
    if (mMidiEvents[channel])
    {
        for (std::vector<MidiEvent>::iterator it = mMidiEvents[channel]->begin(); it != mMidiEvents[channel]->end(); it++)
            events.push_back(*it);
        size = mMidiEvents[channel]->size();
    }
    return size;
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
	bool has = false;

	for (int i = 0; i < MAX_MIDI_CHANNELS; i++)
	{
		if (hasEventsOnChannel(i))
		{
			has = true;
			break;
		}
	}
	return has;
}

double MidiEventHandler::processControlInputs()
{
	double res = 0.0;
	std::vector<MidiEvent> events;

	if (hasEventsOnChannel(0))
	{
		getEvents(0, events);
		res = events[0].getNote();
	}
	return res;
}

void MidiEventHandler::Init()
{
    setModuleName("MidiEventHandler");

    for (int i = 0; i < MAX_MIDI_CHANNELS; i++)
        mMidiEvents[i] = new std::vector<MidiEvent>();
}

void MidiEventHandler::Test()
{
    MidiEvent res;
    bool del;

    TestBeginMsg();
    insertEvent(0, MidiEvent(123, 10));
    dbgOutputF("size: %d", mMidiEvents[0]->size());
    insertEvent(0, MidiEvent(124, 20));
    dbgOutputF("size: %d", mMidiEvents[0]->size());

    del = deleteEvent(0, MidiEvent(123, 0));
    dbgOutputF("delete -> %d", del);
    dbgOutputF("size: %d", mMidiEvents[0]->size());

    res = mMidiEvents[0]->front();
    dbgOutputF("event -> note: %d velocity: %d", res.getNote(), res.getVelocity());
    del = deleteEvent(0, MidiEvent(124, 0));
    dbgOutputF("delete -> %d", del);
    dbgOutputF("size: %d", mMidiEvents[0]->size());
    TestEndMsg();
}
