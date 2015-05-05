#ifndef __ASIOLOADER_H__
#define __ASIOLOADER_H__

#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"

#include <Loader/modWaveLoader.h>
#include <Util/modLogger.h>

#define TEST_RUN_TIME  10.0        // run for 5 seconds

namespace eLibV2
{
    namespace ASIO
    {
        class AsioLoader
        {
        public:
            enum
            {
                // number of input and outputs supported by the host application
                // you can change these to higher or lower values
                kMaxInputChannels = 32,
                kMaxOutputChannels = 32
            };

            // internal data storage
            typedef struct DriverInfo
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
            AsioLoader(AsioDrivers *asiodriver) { m_pAsioDriver = asiodriver; }

            bool LoadDriver(char *name);
            long Init(DriverInfo *asioDriverInfo);
            ASIOError CreateBuffers(DriverInfo *asioDriverInfo);
            void SetupCallbacks();

            ASIOCallbacks asioCallbacks;

            static void bufferSwitch(long index, ASIOBool processNow);
            static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);
            static void sampleRateChanged(ASIOSampleRate sRate);
            static long asioMessages(long selector, long value, void* message, double* opt);

        private:
            AsioDrivers *m_pAsioDriver;
        };
    }
}

#endif
