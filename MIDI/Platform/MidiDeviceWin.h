#ifndef __MIDIDEVICEWIN_H__
#define __MIDIDEVICEWIN_H__

#include <MIDI/BaseMidiDevice.h>

// platform headers
#if defined(WIN32)
#include <Windows.h>
#include <mmsystem.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <Util/Defines.h>
#include <Util/Types.h>

/// TODO: rework
#include <VST/Host/VSTPluginHost.h>

namespace eLibV2
{
    namespace MIDI
    {
        class MidiDeviceWin : public BaseMidiDevice
        {
        public:
            MidiDeviceWin();
            virtual ~MidiDeviceWin() {}

            virtual bool OpenDevice(SInt16 deviceIndex) ;
            virtual void CloseDevice();

            // callback functions
            static void CALLBACK CallbackFunction(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

        protected:
            virtual void EnumerateMidiInDevices();

        private:
            HMIDIIN m_OpenedMidiIn;
        };
    }
}
#endif

#endif
