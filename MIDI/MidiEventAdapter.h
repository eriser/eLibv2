#ifndef MODMIDIEVENTADAPTER_H_
#define MODMIDIEVENTADAPTER_H_

#include <Base/BaseModule.h>
#include <Connection/BaseConnection.h>

#include <MIDI/MidiEvent.h>
#include <MIDI/MidiEventHandler.h>

namespace eLibV2
{
    namespace MIDI
    {
        /**
        adapter for connecting the MidiEventHandler into the module signal flow
        */
        class MidiEventAdapter : public Connection::BaseConnection
        {
        public:
            MidiEventAdapter(MidiEventHandler *handler, SInt16 channel) :
                Base::BaseName("MidiEventAdapter"),
                mChannel(channel),
                mHandler(handler)
            {}

            /**
            process first existing note on selected channel
            */
            virtual double processConnection(void);

        private:
            const SInt16 mChannel; ///< MIDI-channel to check
            MidiEventHandler *mHandler; ///< MidiEventHandler to use
        };
    }
}

#endif
