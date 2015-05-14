#ifndef __ASIODEVICE_H__
#define __ASIODEVICE_H__

#include <ASIO/AsioLoader.h>
#include <vector>

namespace eLibV2
{
    namespace ASIO
    {
        class AsioDevice
        {
        public:
            enum
            {
                MAX_ASIO_DEVICE_NUM = 32,
                MAX_ASIO_DEVICE_NAME = 64
            };

        public:
            AsioDevice();
            ~AsioDevice();

            bool OpenDevice(int deviceIndex);
            void CloseDevice();
            unsigned int GetNumberOfDevices() { return m_uiNumAsioDevices; }
            std::string GetDeviceName(unsigned int deviceIndex);

        private:
            void EnumerateDevices();

        private:
            unsigned int m_uiNumAsioDevices;
            std::vector<std::string> m_DeviceNames;
            AsioLoader *m_AsioLoader;
        };
    }
}

#endif
