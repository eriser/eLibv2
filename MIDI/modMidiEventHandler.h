#ifndef MODMIDIEVENTHANDLER_H_
#define MODMIDIEVENTHANDLER_H_

#include <BASE/modBaseModule.h>
#include <MIDI/modMidiEvent.h>
#include <Util/modLogger.h>

using namespace eLibV2;

namespace eLibV2
{
    class MidiEventHandler : public Base::BaseModule
    {
    public:
        enum { MAX_MIDI_CHANNELS = 16 };

    public:
        MidiEventHandler() : Base::BaseName("MidiEventHandler") { Init(); }
        ~MidiEventHandler();

        void insertEvent(int channel, MidiEvent event);
        bool deleteEvent(int channel, MidiEvent event);
        MidiEventVector getEvents(int channel);
        bool hasEventsOnChannel(int channel);
        bool hasEventsOnAnyChannel();

        void Init();

    private:
        MidiEventVector *mMidiEvents[MAX_MIDI_CHANNELS];
    };
}

#endif
