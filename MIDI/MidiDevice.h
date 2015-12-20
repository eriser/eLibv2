#ifndef __MIDIDEVICE_H__
#define __MIDIDEVICE_H__

// platform headers
#if defined(WIN32)
#include <Windows.h>
#include <mmsystem.h>
#elif defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#include <CoreMIDI/CoreMIDI.h>
#elif defined(__linux__)
#include <alsa/asoundlib.h>
#include <pthread.h>
#include <unistd.h>
#else
#error "unsupported platform"
#endif

#include <iostream>
#include <sstream>
#include <vector>
#include <Util/Defines.h>
#include <Util/Types.h>

/// TODO: rework
#if defined(WIN32)
#include <VST/Host/VSTPluginHost.h>
#endif

namespace eLibV2
{
    namespace MIDI
    {
        class MidiDevice
        {
        public:
#if defined(WIN32)
            MidiDevice(const VST::Host::PluginHost* hostThread);
#else
            MidiDevice();
#endif
            ~MidiDevice() {}

            bool OpenDevice(SInt16 deviceIndex);
            void CloseDevice();
            UInt16 GetNumberOfDevices() { return m_uiNumMidiInDevices; }
            std::string GetDeviceName(UInt16 deviceIndex);

// callback functions
#if defined(WIN32)
            static void CALLBACK CallbackFunction(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
#elif defined(__APPLE__)
            static void CallbackFunction(const MIDIPacketList *packetList, void* readProcRefCon, void* srcConnRefCon);
#elif defined(__linux__)
            static void* CallbackFunction(void *arg);
#else
#error "unsupported platform"
#endif

        private:
            void EnumerateMidiInDevices();

        private:
#if defined(WIN32)
            const VST::Host::PluginHost* m_pHostThread;
            HMIDIIN m_OpenedMidiIn;
#elif defined(__APPLE__)
            MIDIClientRef m_pMidiClient;
            MIDIPortRef m_pMidiInput;
#elif defined(__linux__)
        public:
            pthread_t m_pMidiInputThread;
        private:
            snd_rawmidi_t* m_pMidiInput;
#endif
            UInt16 m_uiNumMidiInDevices;
            std::vector<std::string> m_DeviceNames;
        };
    }
}

#endif
