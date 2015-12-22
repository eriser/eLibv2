#ifndef __BASEMIDIDEVICE_H__
#define __BASEMIDIDEVICE_H__

#include <iostream>
#include <sstream>
#include <vector>
#include <Util/Defines.h>
#include <Util/Types.h>

#include <VST/Host/VSTPluginHost.h>

namespace eLibV2
{
    namespace MIDI
    {
        class BaseMidiDevice
        {
        public:
            BaseMidiDevice();
            virtual ~BaseMidiDevice() {}

            virtual bool OpenDevice(SInt16 deviceIndex) = 0;
            virtual void CloseDevice() = 0;

            UInt16 GetNumberOfDevices() { return m_uiNumMidiInDevices; }
            std::string GetDeviceName(UInt16 deviceIndex);

            /// TODO rework
            virtual void setReceiver(const VST::Host::PluginHost* hostThread) = 0;

        protected:
            virtual void EnumerateMidiInDevices() = 0;

        protected:
            UInt16 m_uiNumMidiInDevices;
            std::vector<std::string> m_DeviceNames;
        };
    }
}

#endif
