/*
* modMidiEventAdapter.h
*
*  Created on: 24.11.2011
*      Author: dedokter
*/

#ifndef MODMIDIEVENTADAPTER_H_
#define MODMIDIEVENTADAPTER_H_

#include <Base/modBaseModule.h>
#include <Blocks/modControlIO.h>

#include <MIDI/modMidiEvent.h>
#include <MIDI/modMidiEventHandler.h>

namespace eLibV2
{
	class MidiEventAdapter : public ControlIO
	{
	public:
		MidiEventAdapter(MidiEventHandler *handler, int channel);

		virtual double processIOs();

	private:
		int mChannel;
		MidiEventHandler *mHandler;
	};
}

#endif
