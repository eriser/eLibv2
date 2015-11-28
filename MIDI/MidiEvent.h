#ifndef MODMIDIEVENT_H_
#define MODMIDIEVENT_H_

#include <vector>

namespace eLibV2
{
    namespace MIDI
    {
        /// enumeration for MIDI-channels
        enum MidiChannels
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
            MIDI_CHANNEL_ALL,
            MIDI_CHANNEL_MAX = MIDI_CHANNEL_ALL
        };
        
        enum MidiVoiceMessages
        {
            NOTE_OFF = 0x80,
            NOTE_ON = 0x90,
            POLY_PRESSURE = 0xA0,
            CONTROL_CHANGE = 0xB0,
            PROGRAM_CHANGE = 0xC0,
            CHANNEL_PRESSURE = 0xD0,
            PITCH_BEND = 0xE0
        };
        
        enum MidiController
        {
            MOD_WHEEL = 0x01,
            VOLUME_CC07 = 0x07,
            PAN_CC10 = 0x0A,
            EXPRESSION_CC11 = 0x0B,
            JOYSTICK_X = 0x10,
            JOYSTICK_Y = 0x11,
            SUSTAIN_PEDAL = 0x40,
            RESET_ALL_CONTROLLERS = 0x79,
            ALL_NOTES_OFF = 0x7B
        };
        
        enum MidiSystemMessages
        {
            SYSTEM_EXCLUSIVE = 0xF0,
            MIDI_TIME_CODE = 0xF1,
            SONG_POSITION_POINTER = 0xF2,
            SONG_SELECT = 0xF3,
            TUNE_REQUEST = 0xF6,
            END_OF_EXCLUSIVE = 0xF7,
            TIMING_CLOCK = 0xF8,
            START = 0xFA,
            CONTINUE = 0xFB,
            SToP = 0xFC,
            ACTIVE_SENSING = 0xFE,
            SYSTEM_RESET = 0xFF
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