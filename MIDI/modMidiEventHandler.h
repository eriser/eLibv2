#ifndef MODMIDIEVENTHANDLER_H_
#define MODMIDIEVENTHANDLER_H_

#include <BASE/modBaseModule.h>
#include <MIDI/modMidiEvent.h>
#include <Util/modLogger.h>

#include <vector>

using namespace eLibV2;

namespace eLibV2
{
    class MidiEventHandler : public BaseModule
    {
	public:
		enum { MAX_MIDI_CHANNELS = 16 };

    public:
		MidiEventHandler() : BaseName("MidiEventHandler") { Init(); }
        ~MidiEventHandler();

        void insertEvent(int channel, MidiEvent event);
        bool deleteEvent(int channel, MidiEvent event);
        int getEvents(int channel, std::vector<MidiEvent>& events);
        bool hasEventsOnChannel(int channel);
		bool hasEventsOnAnyChannel();

        void Init();
        bool Test();

    private:
		typedef std::vector<MidiEvent>::iterator MidiEventsIterator;
        std::vector<MidiEvent> *mMidiEvents[MAX_MIDI_CHANNELS];
    };
}

#endif
