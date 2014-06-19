/*
 * modMidiEventHandler.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODMIDIEVENTHANDLER_H_
#define MODMIDIEVENTHANDLER_H_

#include <vector>

#include "modBaseModule.h"
#include "modMidiEvent.h"

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
        bool hasEvents(int channel);

        void Init();
        void Test();

    private:
        std::vector<MidiEvent> *mMidiEvents[MAX_MIDI_CHANNELS];
    };
}

#endif /* MODMIDIEVENTHANDLER_H_ */
