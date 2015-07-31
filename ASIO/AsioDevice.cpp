#include <ASIO/AsioDevice.h>

using namespace eLibV2::ASIO;
using namespace eLibV2::Loader;
using namespace eLibV2::Host;
using namespace eLibV2::Util::Threads;

// asio.cpp uses this explicitly as external reference (bad design)
AsioDrivers *asioDrivers;

AsioDevice::DriverInfo AsioDevice::ms_asioDriverInfo;
long AsioDevice::ms_processedSamples = 0;

AsioDevice::AsioDevice()
    : m_uiNumAsioDevices(0)
{
    asioDrivers = new AsioDrivers();

    EnumerateDevices();
    EventManager::RegisterEvent(EventManager::EVENT_DATA_WRITTEN);
}

AsioDevice::~AsioDevice()
{
    if (asioDrivers)
    {
        delete asioDrivers;
        asioDrivers = NULL;
    }
    EventManager::UnregisterEvent(EventManager::EVENT_DATA_WRITTEN);
}

void AsioDevice::EnumerateDevices()
{
    char *drivernames[MAX_ASIO_DEVICE_NUM];
    for (int i = 0; i < MAX_ASIO_DEVICE_NUM; i++)
    {
        drivernames[i] = (char*)malloc(MAX_ASIO_DEVICE_NAME);
        memset(drivernames[i], 0, MAX_ASIO_DEVICE_NAME);
    }
    m_uiNumAsioDevices = asioDrivers->getDriverNames((char**)&drivernames, MAX_ASIO_DEVICE_NUM);

    std::cout << "found " << m_uiNumAsioDevices << " ASIO drivers in your system. Please select one for output." << std::endl;
    for (int i = 0; i < m_uiNumAsioDevices; i++)
    {
        m_DeviceNames.push_back(drivernames[i]);
        free(drivernames[i]);
    }
}

std::string AsioDevice::GetDeviceName(unsigned int deviceIndex)
{
    if (deviceIndex < m_DeviceNames.size())
        return m_DeviceNames[deviceIndex];
    else
        return std::string("not found");
}

bool AsioDevice::OpenDevice(int driverIndex)
{
    ModuleLogger::enable();
    ModuleLogger::setDebugView();

    bool bRes = false;
    // load the driver, this will setup all the necessary internal data structures
    // since asio low-level-methods can't handle const char arguments
    if (LoadDriver(const_cast<char*>(m_DeviceNames[driverIndex].c_str())))
    {
        // initialize the driver
        if (ASIOInit(&ms_asioDriverInfo.driverInfo) == ASE_OK)
        {
            ModuleLogger::print(LOG_CLASS_ASIO,
                "asioVersion:   %d\n"
                "driverVersion: %d\n"
                "Name:          %s\n"
                "ErrorMessage:  %s\n",
                ms_asioDriverInfo.driverInfo.asioVersion,
                ms_asioDriverInfo.driverInfo.driverVersion,
                ms_asioDriverInfo.driverInfo.name,
                ms_asioDriverInfo.driverInfo.errorMessage);

            if (Init(&ms_asioDriverInfo) == 0)
            {
                // ASIOControlPanel(); you might want to check wether the ASIOControlPanel() can open
                // ASIOControlPanel();

                if (CreateBuffers(&ms_asioDriverInfo) == ASE_OK)
                {
                    if (ASIOStart() == ASE_OK)
                    {
                        bRes = true;
                        PluginHost::SetBufferFillsize(ms_asioDriverInfo.preferredSize);
                    }
                    else
                        ModuleLogger::print(LOG_CLASS_ASIO, "ASIOStart failed");
                }
                else
                    ModuleLogger::print(LOG_CLASS_ASIO, "CreateBuffers failed");
            }
            else
                ModuleLogger::print(LOG_CLASS_ASIO, "Init failed");
        }
        else
            ModuleLogger::print(LOG_CLASS_ASIO, "ASIOInit failed");
    }
    else
        ModuleLogger::print(LOG_CLASS_ASIO, "LoadDriver failed");
    return bRes;
}

void AsioDevice::CloseDevice()
{
    ASIOStop();
    ASIODisposeBuffers();
    ASIOExit();
    asioDrivers->removeCurrentDriver();
}

#define USE_MANAGED_BUFFER 1

//----------------------------------------------------------------------------------
// conversion from 64 bit ASIOSample/ASIOTimeStamp to double float
#if NATIVE_INT64
#define ASIO64toDouble(a)  (a)
#else
const double twoRaisedTo32 = 4294967296.;
#define ASIO64toDouble(a)  ((a).lo + (a).hi * twoRaisedTo32)
#endif

bool AsioDevice::LoadDriver(char *name)
{
    if (asioDrivers)
        return asioDrivers->loadDriver(name);
    return false;
}

ASIOTime* AsioDevice::bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow)
{
    // store the timeInfo for later use
    ms_asioDriverInfo.tInfo = *timeInfo;

    // get the time stamp of the buffer, not necessary if no
    // synchronization to other media is required
    if (timeInfo->timeInfo.flags & kSystemTimeValid)
        ms_asioDriverInfo.nanoSeconds = ASIO64toDouble(timeInfo->timeInfo.systemTime);
    else
        ms_asioDriverInfo.nanoSeconds = 0;

    if (timeInfo->timeInfo.flags & kSamplePositionValid)
        ms_asioDriverInfo.samples = ASIO64toDouble(timeInfo->timeInfo.samplePosition);
    else
        ms_asioDriverInfo.samples = 0;

    if (timeInfo->timeCode.flags & kTcValid)
        ms_asioDriverInfo.tcSamples = ASIO64toDouble(timeInfo->timeCode.timeCodeSamples);
    else
        ms_asioDriverInfo.tcSamples = 0;

    // buffer size in samples
    long buffSize = ms_asioDriverInfo.preferredSize;

    DWORD wait;
    // send a request for new audio data with the specified buffer size
    EventManager::ResetEvent(EventManager::EVENT_DATA_WRITTEN);
    if (!EventManager::WaitForEvent(EventManager::EVENT_PROCESSING_DONE))
        return 0L;
    EventManager::ResetEvent(EventManager::EVENT_PROCESSING_DONE);

    // perform the processing
    for (int bufferIndex = 0; bufferIndex < ms_asioDriverInfo.inputBuffers + ms_asioDriverInfo.outputBuffers; bufferIndex++)
    {
        // skip inputs
        if (ms_asioDriverInfo.bufferInfos[bufferIndex].isInput)
            continue;

        int channel = ms_asioDriverInfo.bufferInfos[bufferIndex].channelNum;

        int* source = NULL;

#if USE_MANAGED_BUFFER == 1
        // skip channels without data
        ManagedBuffer *managedBuffer = PluginHost::GetOutputBuffer();

        if (channel >= managedBuffer->GetBufferCount())
            continue;

        try
        {
            source = new int[buffSize];
        }
        catch (std::bad_alloc)
        {
            std::cout << "BAD memory allocation error" << std::endl;
            return 0L;
        }
        int readSize = managedBuffer->Read(channel, buffSize, source);
#else
        if (!waveLoader.getWaveData(channel))
            continue;

        source = waveLoader.getWaveData(channel) + (processedSamples % waveLoader.getWaveSize());
#endif
        void* dest = ms_asioDriverInfo.bufferInfos[bufferIndex].buffers[index];

        // OK do processing for the outputs only
        // internal format is float32 -> convert to int32
        SampleConversion::float32toInt32inPlace((float*)source, buffSize);

        switch (ms_asioDriverInfo.channelInfos[bufferIndex].type)
        {
            case ASIOSTInt16LSB:
                SampleConversion::convertMono16LSB((long*)source, (short*)dest, buffSize);
                break;

            case ASIOSTInt24LSB:        // used for 20 bits as well
                SampleConversion::convertMono24LSB((long*)source, (char*)dest, buffSize);
                break;

            case ASIOSTInt32LSB:
                memcpy(dest, source, buffSize * 4);
                break;

            case ASIOSTFloat32LSB:        // IEEE 754 32 bit float, as found on Intel x86 architecture
                memset(ms_asioDriverInfo.bufferInfos[bufferIndex].buffers[index], 0, buffSize * 4);
                break;

            case ASIOSTFloat64LSB:         // IEEE 754 64 bit double float, as found on Intel x86 architecture
                //            convert.convertMono64LSB((long*)source, (char*)dest, buffSize); // TODO FLOAT
                memset(ms_asioDriverInfo.bufferInfos[bufferIndex].buffers[index], 0, buffSize * 8);
                break;

                // these are used for 32 bit data buffer, with different alignment of the data inside
                // 32 bit PCI bus systems can more easily used with these
            case ASIOSTInt32LSB16:        // 32 bit data with 16 bit alignment
            case ASIOSTInt32LSB18:        // 32 bit data with 18 bit alignment
            case ASIOSTInt32LSB20:        // 32 bit data with 20 bit alignment
            case ASIOSTInt32LSB24:        // 32 bit data with 24 bit alignment
                memset(ms_asioDriverInfo.bufferInfos[bufferIndex].buffers[index], 0, buffSize * 4);
                break;

            case ASIOSTInt16MSB:
                SampleConversion::convertMono16MSB((long*)source, (short*)dest, buffSize);
                break;
            case ASIOSTInt24MSB:        // used for 20 bits as well
                SampleConversion::convertMono24MSB((long*)source, (char*)dest, buffSize);
                break;
            case ASIOSTInt32MSB:
                memcpy(dest, source, buffSize * 4);
                SampleConversion::reverseEndian((long*)dest, 4, buffSize);
                break;
            case ASIOSTFloat32MSB:        // IEEE 754 32 bit float, as found on Intel x86 architecture
                memset(ms_asioDriverInfo.bufferInfos[bufferIndex].buffers[index], 0, buffSize * 4);
                break;
            case ASIOSTFloat64MSB:         // IEEE 754 64 bit double float, as found on Intel x86 architecture
                memset(ms_asioDriverInfo.bufferInfos[bufferIndex].buffers[index], 0, buffSize * 8);
                break;

                // these are used for 32 bit data buffer, with different alignment of the data inside
                // 32 bit PCI bus systems can more easily used with these
            case ASIOSTInt32MSB16:        // 32 bit data with 18 bit alignment
            case ASIOSTInt32MSB18:        // 32 bit data with 18 bit alignment
            case ASIOSTInt32MSB20:        // 32 bit data with 20 bit alignment
            case ASIOSTInt32MSB24:        // 32 bit data with 24 bit alignment
                memset(ms_asioDriverInfo.bufferInfos[bufferIndex].buffers[index], 0, buffSize * 4);
                break;
        }
    }

    // finally if the driver supports the ASIOOutputReady() optimization, do it here, all data are in place
    if (ms_asioDriverInfo.postOutput)
        ASIOOutputReady();

    EventManager::SetEvent(EventManager::EVENT_DATA_WRITTEN);
    ms_processedSamples += buffSize;
    return 0L;
}

//----------------------------------------------------------------------------------
void AsioDevice::bufferSwitch(long index, ASIOBool processNow)
{
    // the actual processing callback.
    // Beware that this is normally in a seperate thread, hence be sure that you take care
    // about thread synchronization. This is omitted here for simplicity.

    // as this is a "back door" into the bufferSwitchTimeInfo a timeInfo needs to be created
    // though it will only set the timeInfo.samplePosition and timeInfo.systemTime fields and the according flags
    ASIOTime  timeInfo;
    memset(&timeInfo, 0, sizeof(timeInfo));

    // get the time stamp of the buffer, not necessary if no
    // synchronization to other media is required
    if (ASIOGetSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
        timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

    bufferSwitchTimeInfo(&timeInfo, index, processNow);
}

void AsioDevice::sampleRateChanged(ASIOSampleRate sRate)
{
    // do whatever you need to do if the sample rate changed
    // usually this only happens during external sync.
    // Audio processing is not stopped by the driver, actual sample rate
    // might not have even changed, maybe only the sample rate status of an
    // AES/EBU or S/PDIF digital input at the audio device.
    // You might have to update time/sample related conversion routines, etc.
}

long AsioDevice::asioMessages(long selector, long value, void* message, double* opt)
{
    // currently the parameters "value", "message" and "opt" are not used.
    long ret = 0;
    switch (selector)
    {
        case kAsioSelectorSupported:
            if (value == kAsioResetRequest ||
                value == kAsioEngineVersion ||
                value == kAsioResyncRequest ||
                value == kAsioLatenciesChanged ||
                // the following three were added for ASIO 2.0, you don't necessarily have to support them
                value == kAsioSupportsTimeInfo ||
                value == kAsioSupportsTimeCode ||
                value == kAsioSupportsInputMonitor)
                ret = 1L;
            break;
        case kAsioResetRequest:
            // defer the task and perform the reset of the driver during the next "safe" situation
            // You cannot reset the driver right now, as this code is called from the driver.
            // Reset the driver is done by completely destruct is. I.e. ASIOStop(), ASIODisposeBuffers(), Destruction
            // Afterwards you initialize the driver again.
            ms_asioDriverInfo.stopped;  // In this sample the processing will just stop
            ret = 1L;
            break;
        case kAsioResyncRequest:
            // This informs the application, that the driver encountered some non fatal data loss.
            // It is used for synchronization purposes of different media.
            // Added mainly to work around the Win16Mutex problems in Windows 95/98 with the
            // Windows Multimedia system, which could loose data because the Mutex was hold too long
            // by another thread.
            // However a driver can issue it in other situations, too.
            ret = 1L;
            break;
        case kAsioLatenciesChanged:
            // This will inform the host application that the drivers were latencies changed.
            // Beware, it this does not mean that the buffer sizes have changed!
            // You might need to update internal delay data.
            ret = 1L;
            break;
        case kAsioEngineVersion:
            // return the supported ASIO version of the host application
            // If a host applications does not implement this selector, ASIO 1.0 is assumed
            // by the driver
            ret = 2L;
            break;
        case kAsioSupportsTimeInfo:
            // informs the driver wether the asioCallbacks.bufferSwitchTimeInfo() callback
            // is supported.
            // For compatibility with ASIO 1.0 drivers the host application should always support
            // the "old" bufferSwitch method, too.
            ret = 1;
            break;
        case kAsioSupportsTimeCode:
            // informs the driver wether application is interested in time code info.
            // If an application does not need to know about time code, the driver has less work
            // to do.
            ret = 0;
            break;
    }
    return ret;
}

long AsioDevice::Init(DriverInfo *ms_asioDriverInfo)
{
    // collect the informational data of the driver
    // get the number of available channels
    if (ASIOGetChannels(&ms_asioDriverInfo->inputChannels, &ms_asioDriverInfo->outputChannels) == ASE_OK)
    {
        printf("ASIOGetChannels (inputs: %d, outputs: %d);\n", ms_asioDriverInfo->inputChannels, ms_asioDriverInfo->outputChannels);

        // get the usable buffer sizes
        if (ASIOGetBufferSize(&ms_asioDriverInfo->minSize, &ms_asioDriverInfo->maxSize,
            &ms_asioDriverInfo->preferredSize, &ms_asioDriverInfo->granularity) == ASE_OK)
        {
            printf("ASIOGetBufferSize (min: %d, max: %d, preferred: %d, granularity: %d);\n",
                ms_asioDriverInfo->minSize, ms_asioDriverInfo->maxSize,
                ms_asioDriverInfo->preferredSize, ms_asioDriverInfo->granularity);

            // get the currently selected sample rate
            if (ASIOGetSampleRate(&ms_asioDriverInfo->sampleRate) == ASE_OK)
            {
                printf("ASIOGetSampleRate (sampleRate: %f);\n", ms_asioDriverInfo->sampleRate);
                if (ms_asioDriverInfo->sampleRate <= 0.0 || ms_asioDriverInfo->sampleRate > 96000.0)
                {
                    // Driver does not store it's internal sample rate, so set it to a know one.
                    // Usually you should check beforehand, that the selected sample rate is valid
                    // with ASIOCanSampleRate().
                    if (ASIOSetSampleRate(44100.0) == ASE_OK)
                    {
                        if (ASIOGetSampleRate(&ms_asioDriverInfo->sampleRate) == ASE_OK)
                            printf("ASIOGetSampleRate (sampleRate: %f);\n", ms_asioDriverInfo->sampleRate);
                        else
                            return -6;
                    }
                    else
                        return -5;
                }

                // check wether the driver requires the ASIOOutputReady() optimization
                // (can be used by the driver to reduce output latency by one block)
                if (ASIOOutputReady() == ASE_OK)
                    ms_asioDriverInfo->postOutput = true;
                else
                    ms_asioDriverInfo->postOutput = false;
                printf("ASIOOutputReady(); - %s\n", ms_asioDriverInfo->postOutput ? "Supported" : "Not supported");

                return 0;
            }
            return -3;
        }
        return -2;
    }
    return -1;
}

void AsioDevice::SetupCallbacks()
{
    asioCallbacks.asioMessage = &asioMessages;
    asioCallbacks.sampleRateDidChange = &sampleRateChanged;
    asioCallbacks.bufferSwitch = &bufferSwitch;
    asioCallbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;
}

ASIOError AsioDevice::CreateBuffers(DriverInfo *ms_asioDriverInfo)
{
    // create buffers for all inputs and outputs of the card with the 
    // preferredSize from ASIOGetBufferSize() as buffer size
    long i;
    ASIOError result;

    // fill the bufferInfos from the start without a gap
    ASIOBufferInfo *info = ms_asioDriverInfo->bufferInfos;

    SetupCallbacks();

    // prepare inputs (Though this is not necessaily required, no opened inputs will work, too
    if (ms_asioDriverInfo->inputChannels > kMaxInputChannels)
        ms_asioDriverInfo->inputBuffers = kMaxInputChannels;
    else
        ms_asioDriverInfo->inputBuffers = ms_asioDriverInfo->inputChannels;
    for (i = 0; i < ms_asioDriverInfo->inputBuffers; i++, info++)
    {
        info->isInput = ASIOTrue;
        info->channelNum = i;
        info->buffers[0] = info->buffers[1] = 0;
    }

    // prepare outputs
    if (ms_asioDriverInfo->outputChannels > kMaxOutputChannels)
        ms_asioDriverInfo->outputBuffers = kMaxOutputChannels;
    else
        ms_asioDriverInfo->outputBuffers = ms_asioDriverInfo->outputChannels;
    for (i = 0; i < ms_asioDriverInfo->outputBuffers; i++, info++)
    {
        info->isInput = ASIOFalse;
        info->channelNum = i;
        info->buffers[0] = info->buffers[1] = 0;
    }

    // create and activate buffers
    result = ASIOCreateBuffers(ms_asioDriverInfo->bufferInfos,
        ms_asioDriverInfo->inputBuffers + ms_asioDriverInfo->outputBuffers,
        ms_asioDriverInfo->preferredSize, &asioCallbacks);
    if (result == ASE_OK)
    {
        // now get all the buffer details, sample word length, name, word clock group and activation
        for (i = 0; i < ms_asioDriverInfo->inputBuffers + ms_asioDriverInfo->outputBuffers; i++)
        {
            ms_asioDriverInfo->channelInfos[i].channel = ms_asioDriverInfo->bufferInfos[i].channelNum;
            ms_asioDriverInfo->channelInfos[i].isInput = ms_asioDriverInfo->bufferInfos[i].isInput;
            result = ASIOGetChannelInfo(&ms_asioDriverInfo->channelInfos[i]);
            if (result != ASE_OK)
                break;
        }

        if (result == ASE_OK)
        {
            // get the input and output latencies
            // Latencies often are only valid after ASIOCreateBuffers()
            // (input latency is the age of the first sample in the currently returned audio block)
            // (output latency is the time the first sample in the currently returned audio block requires to get to the output)
            result = ASIOGetLatencies(&ms_asioDriverInfo->inputLatency, &ms_asioDriverInfo->outputLatency);
            if (result == ASE_OK)
                printf("ASIOGetLatencies (input: %d, output: %d);\n", ms_asioDriverInfo->inputLatency, ms_asioDriverInfo->outputLatency);
        }
    }
    return result;
}

