#ifndef MODMIDIEVENTADAPTER_H_
#define MODMIDIEVENTADAPTER_H_

#include <Base/modBaseModule.h>
#include <Base/modBaseConnection.h>

#include <MIDI/modMidiEvent.h>
#include <MIDI/modMidiEventHandler.h>

using namespace eLibV2;

namespace eLibV2
{
	class MidiEventAdapter : public BaseConnection
	{
	public:
		MidiEventAdapter::MidiEventAdapter(MidiEventHandler *handler, int channel) : BaseName("MidiEventAdapter"), mHandler(handler), mChannel(channel) {}

		/* process first existing note on selected channel */
		virtual double processConnection();

	private:
		const int mChannel;
		MidiEventHandler *mHandler;
	};
}

#endif
