#ifndef __MIDIDEVICE_H__
#define __MIDIDEVICE_H__

#ifdef WIN32

#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <sstream>
#include <vector>

#include <Util/Defines.h>
#include <Plugin/PluginHost.h>

namespace eLibV2
{
    namespace MIDI
    {
        class MidiDevice
        {
        public:
            MidiDevice(const Host::PluginHost* hostThread);
            ~MidiDevice() {}

            bool OpenDevice(int deviceIndex);
            void CloseDevice();
            unsigned int GetNumberOfDevices() { return m_uiNumMidiInDevices; }
            std::string GetDeviceName(unsigned int deviceIndex);

            static void CALLBACK CallbackFunction(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

        private:
            void EnumerateMidiInDevices();

        private:
            const Host::PluginHost* m_pHostThread;
            HMIDIIN m_OpenedMidiIn;
            unsigned int m_uiNumMidiInDevices;
            std::vector<std::string> m_DeviceNames;
        };
    }
}

#endif

#endif
