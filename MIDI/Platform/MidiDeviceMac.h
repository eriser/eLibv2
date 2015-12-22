#ifndef __MIDIDEVICEMAC_H__
#define __MIDIDEVICEMAC_H__

#if defined(__APPLE__)
#include <MIDI/BaseMidiDevice.h>

// platform headers
#include <CoreServices/CoreServices.h>
#include <CoreMIDI/CoreMIDI.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <Util/Defines.h>
#include <Util/Types.h>

namespace eLibV2
{
    namespace MIDI
    {
        class MidiDeviceMac : public BaseMidiDevice
        {
        public:
            MidiDeviceMac();
            virtual ~MidiDeviceMac() {}

            virtual bool OpenDevice(SInt16 deviceIndex);
            virtual void CloseDevice();

            void setReceiver(const VST::Host::PluginHost* hostThread) {}

            // callback functions
            static void CallbackFunction(const MIDIPacketList *packetList, void* readProcRefCon, void* srcConnRefCon);

        protected:
            virtual void EnumerateMidiInDevices();

        private:
            MIDIClientRef m_pMidiClient;
            MIDIPortRef m_pMidiInput;
        };
    }
}
#endif

#endif
