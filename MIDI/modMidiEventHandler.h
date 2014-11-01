/*
 * modMidiEventHandler.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODMIDIEVENTHANDLER_H_
#define MODMIDIEVENTHANDLER_H_

#include <BASE/modBaseModule.h>
#include <MIDI/modMidiEvent.h>

#include <vector>

#define MAX_MIDI_CHANNELS 16

namespace eLibV2
{
    class MidiEventHandler : public BaseModule
    {
    public:
        MidiEventHandler();
        ~MidiEventHandler();

        void insertEvent(int channel, MidiEvent event);
        bool deleteEvent(int channel, MidiEvent event);
        int getEvents(int channel, std::vector<MidiEvent>& events);
        bool hasEventsOnChannel(int channel);
		bool hasEventsOnAnyChannel();

        void Init();
        void Test();

    private:
        std::vector<MidiEvent> *mMidiEvents[MAX_MIDI_CHANNELS];
    };
}

#endif /* MODMIDIEVENTHANDLER_H_ */
