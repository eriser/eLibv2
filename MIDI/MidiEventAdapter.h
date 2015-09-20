#ifndef MODMIDIEVENTADAPTER_H_
#define MODMIDIEVENTADAPTER_H_

#include <Base/BaseModule.h>
#include <Base/BaseConnection.h>

#include <MIDI/MidiEvent.h>
#include <MIDI/MidiEventHandler.h>

namespace eLibV2
{
    namespace MIDI
    {
        /**
        adapter for connecting the MidiEventHandler into the module signal flow
        */
        class MidiEventAdapter : public Base::BaseConnection
        {
        public:
            MidiEventAdapter::MidiEventAdapter(MidiEventHandler *handler, int channel) :
                Base::BaseName("MidiEventAdapter"),
                mHandler(handler),
                mChannel(channel)
            {}

            /**
            process first existing note on selected channel
            */
            virtual double processConnection();

        private:
            const int mChannel; ///< MIDI-channel to check
            MidiEventHandler *mHandler; ///< MidiEventHandler to use
        };
    }
}

#endif
