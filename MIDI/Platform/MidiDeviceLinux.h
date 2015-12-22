#ifndef __MIDIDEVICELINUX_H__
#define __MIDIDEVICELINUX_H__

#if defined(__linux__)

#include <MIDI/BaseMidiDevice.h>

// platform headers
#include <alsa/asoundlib.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <Util/Defines.h>
#include <Util/Types.h>

namespace eLibV2
{
    namespace MIDI
    {
        class MidiDeviceLinux : public BaseMidiDevice
        {
        public:
            MidiDeviceLinux();
            virtual ~MidiDeviceLinux() {}

            virtual bool OpenDevice(SInt16 deviceIndex);
            virtual void CloseDevice();

            void setReceiver(const VST::Host::PluginHost* hostThread) {}

            // callback functions
            static void* CallbackFunction(void *arg);

        protected:
            virtual void EnumerateMidiInDevices();

        public:
            pthread_t m_pMidiInputThread;
        private:
            snd_rawmidi_t* m_pMidiInput;
        };
    }
}

#endif

#endif
