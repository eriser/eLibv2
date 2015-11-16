#ifndef __MIDIDEVICE_H__
#define __MIDIDEVICE_H__

#ifdef WIN32

#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <sstream>
#include <vector>

#include <Util/Defines.h>
#include <VST/Host/VSTPluginHost.h>

namespace eLibV2
{
    namespace MIDI
    {
        class MidiDevice
        {
        public:
            MidiDevice(const VST::Host::PluginHost* hostThread);
            ~MidiDevice() {}

            bool OpenDevice(SInt16 deviceIndex);
            void CloseDevice();
            UInt16 GetNumberOfDevices() { return m_uiNumMidiInDevices; }
            std::string GetDeviceName(UInt16 deviceIndex);

            static void CALLBACK CallbackFunction(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

        private:
            void EnumerateMidiInDevices();

        private:
            const VST::Host::PluginHost* m_pHostThread;
            HMIDIIN m_OpenedMidiIn;
            UInt16 m_uiNumMidiInDevices;
            std::vector<std::string> m_DeviceNames;
        };
    }
}

#endif

#endif
