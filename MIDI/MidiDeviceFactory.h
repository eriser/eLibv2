#ifndef __MIDIDEVICEFACTORY_H__
#define __MIDIDEVICEFACTORY_H__

#include <MIDI/BaseMidiDevice.h>
#include <MIDI/Platform/MidiDeviceLinux.h>
#include <MIDI/Platform/MidiDeviceMac.h>
#include <MIDI/Platform/MidiDeviceWin.h>

#include <Util/Logger.h>

using namespace eLibV2::MIDI;
using namespace eLibV2::Util;

namespace eLibV2
{
    namespace MIDI
    {
        class MidiDeviceFactory
        {
        public:
            ~MidiDeviceFactory() {}

            static BaseMidiDevice* GetMidiDevice()
            {
                if (m_pInstance == NULL)
                    m_pInstance = new MidiDeviceFactory();
                return m_pInstance->GetMidiDeviceInternal();
            }
            
        private:
            MidiDeviceFactory()
            {
#if defined(WIN32)
                m_pMidiDevice = new MidiDeviceWin();
#elif defined(__APPLE__)
                m_pMidiDevice = new MidiDeviceMac();
#elif defined(__linux__)
                m_pMidiDevice = new MidiDeviceLinux();
#else
                ModuleLogger::print(LOG_CLASS_MIDI, "unsupported platform");
#endif
            }

            BaseMidiDevice* GetMidiDeviceInternal()
            {
                return m_pMidiDevice;
            }
            
        private:
            static MidiDeviceFactory* m_pInstance;
            BaseMidiDevice* m_pMidiDevice;
        };
    }
}

#endif
