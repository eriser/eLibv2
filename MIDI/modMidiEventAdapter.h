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
		MidiEventAdapter::MidiEventAdapter(MidiEventHandler *handler, int channel) : BaseName("MidiEventAdapter"), mHandler(handler), mChannel(channel) {}

		/* process first existing note on selected channel */
		virtual double processIOs();

	private:
		int mChannel;
		MidiEventHandler *mHandler;
	};
}

#endif
