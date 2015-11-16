#ifndef __ASIODEVICE_H__
#define __ASIODEVICE_H__

#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"

#include <ASIO/AsioDevice.h>
#include <ASIO/SampleConversion.h>
#include <VST/Host/VSTPluginHost.h>
#include <Loader/WaveLoader.h>
#include <Util/Logger.h>
#include <Util/Threads/EventManager.h>

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
                // number of input and outputs supported by the host application
                // you can change these to higher or lower values
                kMaxInputChannels = 4,
                kMaxOutputChannels = 4,
                MAX_ASIO_DEVICE_NUM = 32,
                MAX_ASIO_DEVICE_NAME = 64
            };

            // internal data storage
            struct DriverInfo
            {
                // ASIOInit()
                ASIODriverInfo driverInfo;

                // ASIOGetChannels()
                long           inputChannels;
                long           outputChannels;

                // ASIOGetBufferSize()
                long           minSize;
                long           maxSize;
                long           preferredSize;
                long           granularity;

                // ASIOGetSampleRate()
                ASIOSampleRate sampleRate;

                // ASIOOutputReady()
                bool           postOutput;

                // ASIOGetLatencies ()
                long           inputLatency;
                long           outputLatency;

                // ASIOCreateBuffers ()
                long inputBuffers;    // becomes number of actual created input buffers
                long outputBuffers;    // becomes number of actual created output buffers
                ASIOBufferInfo bufferInfos[kMaxInputChannels + kMaxOutputChannels]; // buffer info's

                // ASIOGetChannelInfo()
                ASIOChannelInfo channelInfos[kMaxInputChannels + kMaxOutputChannels]; // channel info's
                // The above two arrays share the same indexing, as the data in them are linked together

                // Information from ASIOGetSamplePosition()
                // data is converted to double floats for easier use, however 64 bit integer can be used, too
                double         nanoSeconds;
                double         samples;
                double         tcSamples;    // time code samples

                // bufferSwitchTimeInfo()
                ASIOTime       tInfo;            // time info state

                // Signal the end of processing in this example
                bool           stopped;
            };

        public:
            AsioDevice();
            ~AsioDevice();

            bool OpenDevice(int deviceIndex);
            void CloseDevice();
            UInt16 GetNumberOfDevices() { return m_uiNumAsioDevices; }
            std::string GetDeviceName(UInt16 deviceIndex);

            // callbacks directly called from hardware driver
            static void bufferSwitch(long index, ASIOBool processNow);
            static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);
            static void sampleRateChanged(ASIOSampleRate sRate);
            static long asioMessages(long selector, long value, void* message, double* opt);

        public:
            ASIOCallbacks asioCallbacks;
            static void *externalBuffer[kMaxOutputChannels];
            static DriverInfo ms_asioDriverInfo;
            static long ms_processedSamples;

        private:
            void EnumerateDevices();
            bool LoadDriver(char *name);
            long Init(DriverInfo *asioDriverInfo);
            ASIOError CreateBuffers(DriverInfo *asioDriverInfo);
            void SetupCallbacks();

        private:
            UInt16 m_uiNumAsioDevices;
            std::vector<std::string> m_DeviceNames;
        };
    }
}

#endif
