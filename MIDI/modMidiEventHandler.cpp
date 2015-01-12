#include <MIDI/modMidiEventHandler.h>

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
	int size = 0;
	if (mMidiEvents[channel])
	{
		for (MidiEventsIterator it = mMidiEvents[channel]->begin(); it != mMidiEvents[channel]->end(); it++)
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
		mMidiEvents[i] = new std::vector<MidiEvent>();
}

bool MidiEventHandler::Test()
{
	MidiEvent res;
	bool del;

	TestBeginMsg();
	insertEvent(0, MidiEvent(123, 10));
	ModuleLogger::print("size: %d", mMidiEvents[0]->size());
	insertEvent(0, MidiEvent(124, 20));
	ModuleLogger::print("size: %d", mMidiEvents[0]->size());

	del = deleteEvent(0, MidiEvent(123, 0));
	ModuleLogger::print("delete -> %d", del);
	ModuleLogger::print("size: %d", mMidiEvents[0]->size());

	res = mMidiEvents[0]->front();
	ModuleLogger::print("event -> note: %d velocity: %d", res.getNote(), res.getVelocity());
	del = deleteEvent(0, MidiEvent(124, 0));
	ModuleLogger::print("delete -> %d", del);
	ModuleLogger::print("size: %d", mMidiEvents[0]->size());
	TestEndMsg();

	return true;
}
