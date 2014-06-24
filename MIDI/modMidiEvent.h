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
