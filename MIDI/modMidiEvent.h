#ifndef MODMIDIEVENT_H_
#define MODMIDIEVENT_H_

#include <vector>

namespace eLibV2
{
    namespace MIDI
    {
        /**
        class for creating MidiEvents with note and velocity stored internally
        */
        class MidiEvent
        {
        public:
            enum
            {
                MIDI_CHANNEL_1 = 0,
                MIDI_CHANNEL_2,
                MIDI_CHANNEL_3,
                MIDI_CHANNEL_4,
                MIDI_CHANNEL_5,
                MIDI_CHANNEL_6,
                MIDI_CHANNEL_7,
                MIDI_CHANNEL_8,
                MIDI_CHANNEL_9,
                MIDI_CHANNEL_10,
                MIDI_CHANNEL_11,
                MIDI_CHANNEL_12,
                MIDI_CHANNEL_13,
                MIDI_CHANNEL_14,
                MIDI_CHANNEL_15,
                MIDI_CHANNEL_16,
                MIDI_CHANNEL_MAX
            };

        public:
            /**
            create event with given note and velocity
            */
            MidiEvent(int note, int velocity) { mNote = note; mVelocity = velocity; }

            /**
            get note stored in event
            @return note
            */
            int getNote() { return mNote; }

            /**
            get velocity stored in event
            @return velocity
            */
            int getVelocity() { return mVelocity; }

        private:
            MidiEvent() {}

        private:
            int mNote; ///< note
            int mVelocity; ///< velocity
        };

        typedef std::vector<MidiEvent> MidiEventVector; ///< typedef for vector of events
        typedef std::vector<MidiEvent>::iterator MidiEventIterator; ///< typedef for iterator of events
    }
}

#endif