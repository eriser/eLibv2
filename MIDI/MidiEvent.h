#ifndef MODMIDIEVENT_H_
#define MODMIDIEVENT_H_

#include <vector>

namespace eLibV2
{
    namespace MIDI
    {
        /// enumeration for MIDI-channels
        enum
        {
            MIDI_CHANNEL_MIN = 0,
            MIDI_CHANNEL_1 = MIDI_CHANNEL_MIN,
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

        /**
        class for creating MidiEvents with note and velocity stored internally
        */
        class MidiEvent
        {
        public:
            /**
            create event with given note and velocity
            */
            MidiEvent(SInt16 note, SInt16 velocity) { mNote = note; mVelocity = velocity; }

            /**
            get note stored in event
            @return note
            */
            SInt16 getNote() const { return mNote; }

            /**
            get velocity stored in event
            @return velocity
            */
            SInt16 getVelocity() const { return mVelocity; }

        private:
            MidiEvent() {}

        private:
            SInt16 mNote; ///< note
            SInt16 mVelocity; ///< velocity
        };

        typedef std::vector<MidiEvent> MidiEventVector; ///< typedef for vector of events
        typedef std::vector<MidiEvent>::iterator MidiEventIterator; ///< typedef for iterator of events
        typedef std::vector<MidiEvent>::const_iterator ConstMidiEventIterator; ///< typedef for iterator of events
    }
}

#endif