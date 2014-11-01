/*
 * modMidiEvent.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODMIDIEVENT_H_
#define MODMIDIEVENT_H_

namespace eLibV2
{
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
        MidiEvent() { mNote = 0; mVelocity = 0; }
        MidiEvent(int note, int velocity) { mNote = note; mVelocity = velocity; }
        int getNote() { return mNote; }
        int getVelocity() { return mVelocity; }

    private:
        int mNote;
        int mVelocity;
    };
}

#endif /* MODMIDIEVENT_H_ */
