#include <Plugin/PluginHost.h>
#include <ASIO/AsioDevice.h>

using namespace eLibV2::Host;
using namespace eLibV2::Util::Threads;

double PluginHost::ms_dSamplerate = kSampleRate;
double PluginHost::ms_dTempo = 140.0f;
unsigned int PluginHost::ms_uiBlocksize = kBlockSize;
unsigned int PluginHost::ms_uiTimeSignatureBeatsPerMeasure = 4;
unsigned int PluginHost::ms_uiTimeSignatureNoteValue = 4;
VstTimeInfo PluginHost::_VstTimeInfo;
bool PluginHost::ms_bTransportPlaying = false;
LARGE_INTEGER PluginHost::ms_liElapsedMicroseconds;
ManagedBuffer* PluginHost::ms_managedBuffer = NULL;
int PluginHost::ms_iBufferRequestedSize = 0;
long PluginHost::ms_SamplesProcessed = 0;

PluginHost::PluginHost() : m_NumLoadedPlugins(0)
{
    EventManager::RegisterEvent(EventManager::EVENT_PROCESSING_DONE);
}

PluginHost::~PluginHost()
{
    EventManager::UnregisterEvent(EventManager::EVENT_PROCESSING_DONE);
}

bool PluginHost::OpenPlugin(std::string fileName)
{
    std::cout << "HOST> Load VST plugin '" << fileName << "'" << std::endl;
    PluginInterface *plugin = new PluginInterface();

    if (plugin->Load(fileName, PluginHost::HostCallback))
    {
        m_LoadedPlugins[plugin->GetPluginID()] = plugin;

        plugin->PrintProperties();
//        plugin->PrintPrograms();
//        plugin->PrintParameters();
        plugin->PrintCapabilities();
//        plugin->ProcessReplacing();

        return true;
    }
    else
        std::cout << "Failed to load VST Plugin library!" << std::endl;
    return false;
}

void PluginHost::ClosePlugin(std::string pluginID)
{
    if (m_LoadedPlugins.count(pluginID) > 0)
    {
        m_LoadedPlugins[pluginID]->Unload();
        delete m_LoadedPlugins[pluginID];
        m_LoadedPlugins.erase(pluginID);
    }
}

void PluginHost::CloseAll()
{
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        (*it).second->Unload();
    m_LoadedPlugins.clear();
}

void PluginHost::StartAll()
{
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        (*it).second->Start();
    ms_bTransportPlaying = true;
    StartTimer();
}

void PluginHost::StopAll()
{
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        (*it).second->Stop();
    ms_bTransportPlaying = false;
    StopTimer();
}

void PluginHost::StartTimer()
{
    QueryPerformanceFrequency(&m_liFrequency);
    QueryPerformanceCounter(&m_liStartingTime);
}

void PluginHost::StopTimer()
{
    QueryPerformanceCounter(&m_liEndingTime);
    ms_liElapsedMicroseconds.QuadPart = m_liEndingTime.QuadPart - m_liStartingTime.QuadPart;
    ms_liElapsedMicroseconds.QuadPart *= 1000000;
    ms_liElapsedMicroseconds.QuadPart /= m_liFrequency.QuadPart;
}

VstInt32 CanHostDo(char *canDo)
{
    static const char* hostCanDos[] =
    {
        "sendVstEvents",
        "sendVstMidiEvent",
        "sendVstTimeInfo",
        "receiveVstEvents",
        "receiveVstMidiEvent",
        "receiveVstTimeInfo",
        "acceptIOChanges",
        "sizeWindow",
        "asyncProcessing",
        "offline",
        "supplyIdle",
        "supportShell",
        "openFileSelector",
        "editFile",
        "closeFileSelector"
    };

    VstInt32 res = -1;
    for (VstInt32 canDoIndex = 0; canDoIndex < sizeof(hostCanDos) / sizeof(hostCanDos[0]); canDoIndex++)
    {
        if (strcmp(canDo, hostCanDos[canDoIndex]) == 0)
        {
            res = 1;
            break;
        }
    }
    return res;
}

PluginInterfaceList PluginHost::GetLoadedPlugins()
{
    PluginInterfaceList list;
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        list.push_back((*it).second);
    return list;
}

PluginInterfaceList PluginHost::GetPluginsForMidiChannel(int channel)
{
    PluginInterfaceList list;
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
    {
        PluginInterface *p = (*it).second;
        if (p->GetMidiChannel() == channel)
            list.push_back(p);
    }
    return list;
}

void PluginHost::InsertMidiEvent(int channel, int status, int data1, int data2)
{
    PluginInterfaceList list = GetPluginsForMidiChannel(channel);

    for (PluginInterfaceList::iterator it = list.begin(); it != list.end(); it++)
    {
        PluginInterface *plugin = (*it);
        if (plugin)
            plugin->SendMidi(channel, status, data1, data2);
    }
}

VstIntPtr VSTCALLBACK PluginHost::HostCallback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
    VstIntPtr result = 0;

    char pluginID[5] = "????";
    if (effect)
        PluginInterface::GetPluginStringFromLong(effect->uniqueID, pluginID);

    if (opcode && opcode != audioMasterIdle && opcode != audioMasterGetTime)
        std::cout << "HOST> '" << pluginID << "': ";

#if 0
    // Filter idle calls...
    bool filtered = false;
    if (opcode == audioMasterIdle)
    {
        static bool wasIdle = false;
        if (wasIdle)
            filtered = true;
        else
        {
            std::cout << "(Future idle calls will not be displayed!)" << std::endl;
            wasIdle = true;
        }
    }

    if (!filtered)
    {
        std::cout << "PLUG> HostCallback (opcode " << (int)opcode << ") index: " << (int)index << " value: " << (int)FromVstPtr<void>(value) << " ptr: " << ptr << " opt " << opt << std::endl;
    }
#endif

    switch (opcode)
    {
    case audioMasterAutomate:
        //        std::cout << "received change for parameter " << index << " value: " << opt << std::endl;
        break;

    case audioMasterVersion:
        result = kVstVersion;
        break;

    case audioMasterCurrentId:
        result = 1;
        break;

    case audioMasterIdle:
        break;

    case audioMasterGetTime:
//        std::cout << "requesting VstTimeInfo: " << std::hex << value << std::dec << std::endl;

        // These values are always valid
        _VstTimeInfo.samplePos = 0;
        _VstTimeInfo.sampleRate = GetSampleRate();

        // Set flags for transport state
        _VstTimeInfo.flags = 0;
        // TODO:        if (HasTransportChanged())
        //_VstTimeInfo.flags |= kVstTransportChanged;
        if (IsTransportPlaying())
            _VstTimeInfo.flags |= kVstTransportPlaying;

        // Fill values based on other flags which may have been requested
        if (value & kVstNanosValid)
        {
            // It doesn't make sense to return this value, as the plugin may try to calculate
            // something based on the current system time. As we are running offline, anything
            // the plugin calculates here will probably be wrong given the way we are running.
            // However, for realtime mode, this flag should be implemented in that case.
            _VstTimeInfo.nanoSeconds = 0;
            _VstTimeInfo.flags |= kVstNanosValid;
        }

        if (value & kVstPpqPosValid)
        {
            // TODO: Move calculations to AudioClock
            double samplesPerBeat = (60.0 / GetTempo()) * GetSampleRate();
            // Musical time starts with 1, not 0
            _VstTimeInfo.ppqPos = (_VstTimeInfo.samplePos / samplesPerBeat) + 1.0;
            _VstTimeInfo.flags |= kVstPpqPosValid;
        }

        if (value & kVstTempoValid)
        {
            _VstTimeInfo.tempo = GetTempo();
            _VstTimeInfo.flags |= kVstTempoValid;
        }

        if (value & kVstBarsValid)
        {
            if (!(value & kVstPpqPosValid))
                std::cout << "Plugin requested position in bars, but not PPQ" << std::endl;

            // TODO: Move calculations to AudioClock
            double currentBarPos = floor(_VstTimeInfo.ppqPos / GetTimeSignatureBeatsPerMeasure());
            _VstTimeInfo.barStartPos = currentBarPos * GetTimeSignatureBeatsPerMeasure() + 1.0;
//            std::cout << "Current bar is " << _VstTimeInfo.barStartPos << std::endl;
            _VstTimeInfo.flags |= kVstBarsValid;
        }

        if (value & kVstCyclePosValid)
        {
            _VstTimeInfo.cycleStartPos = 1;
            _VstTimeInfo.cycleEndPos = 4;
            _VstTimeInfo.flags |= kVstCyclePosValid;
        }

        if (value & kVstTimeSigValid)
        {
            _VstTimeInfo.timeSigNumerator = GetTimeSignatureBeatsPerMeasure();
            _VstTimeInfo.timeSigDenominator = GetTimeSignatureNoteValue();
            _VstTimeInfo.flags |= kVstTimeSigValid;
        }

        if (value & kVstSmpteValid)
            std::cout << "Current time in SMPTE format" << std::endl;

        if (value & kVstClockValid)
            std::cout << "Sample frames until next clock" << std::endl;

        result = (VstIntPtr)&_VstTimeInfo;
        break;

    case audioMasterProcessEvents:
        std::cout << "events from plugin received " << std::hex << ptr << std::dec << std::endl;
        break;

    case audioMasterIOChanged:
        std::cout << "number of inputs/outputs changing not supported" << std::endl;
        result = 0;
        break;

    case audioMasterSizeWindow:
        std::cout << "editor size changed. width: " << index << " height: " << value << std::endl;
        result = 0;
        break;

    case audioMasterGetSampleRate:
        std::cout << "samplerate requested." << std::endl;
        result = (int)GetSampleRate();
        break;

    case audioMasterGetBlockSize:
        std::cout << "blocksize requested." << std::endl;
        result = (VstIntPtr)GetBlocksize();
        break;

    case audioMasterGetInputLatency:
        result = 0;
        break;

    case audioMasterGetOutputLatency:
        result = 0;
        break;

    case audioMasterGetCurrentProcessLevel:
        result = kVstProcessLevelRealtime;
        break;

    case audioMasterGetAutomationState:
        result = kVstAutomationOff;
        break;

    case audioMasterGetVendorString:
        std::cout << "vendor string requested" << std::endl;
        strncpy((char*)ptr, "e:fope media", kVstMaxVendorStrLen);
        break;

    case audioMasterGetProductString:
        std::cout << "product string requested" << std::endl;
        strncpy((char*)ptr, "vst testhost", kVstMaxProductStrLen);
        break;

    case audioMasterGetVendorVersion:
        std::cout << "product version requested" << std::endl;
        result = 1;
        break;

    case audioMasterVendorSpecific:
        std::cout << "vendor specific requested: " << index << " value: " << value << " ptr: " << std::hex << ptr << std::dec << " opt: " << opt << std::endl;
        result = 0;
        break;

    case audioMasterCanDo:
        std::cout << "request for master canDo '" << (char*)ptr << "'" << std::endl;
        result = CanHostDo((char*)ptr);
        break;

    case audioMasterGetLanguage:
        std::cout << "host language requested" << std::endl;
        result = kVstLangEnglish;
        break;

    case audioMasterUpdateDisplay:
        std::cout << "display updated" << std::endl;
        break;

    case audioMasterBeginEdit:
        std::cout << "editing parameter " << index << std::endl;
        break;

    case audioMasterEndEdit:
        std::cout << "ended editing parameter " << index << std::endl;
        break;

    case audioMasterOpenFileSelector:
        std::cout << "ptr: " << std::hex << ptr << std::dec << std::endl;
        result = 0;
        break;

    case audioMasterCloseFileSelector:
        std::cout << "ptr: " << std::hex << ptr << std::dec << std::endl;
        result = 0;
        break;

    default:
        std::cout << "opcode received: " << opcode << " index: " << index << " value: " << value << " ptr: " << std::hex << ptr << std::dec << " opt: " << opt << std::endl;
        break;
    }

    return result;
}

DWORD WINAPI PluginHost::ProcessReplacing(LPVOID lpParam)
{
    extern bool stopProcessing;
    PluginInterfaceList *pList = reinterpret_cast<PluginInterfaceList*>(lpParam);

    // get number of inputs/outputs -> these will NOT change during lifetime
    int m_uiNumInputs = 0;
    int m_uiNumOutputs = 0;
    for (PluginInterfaceList::iterator it = pList->begin(); it != pList->end(); it++)
    {
        if (m_uiNumInputs < (*it)->GetEffect()->numInputs)
            m_uiNumInputs = (*it)->GetEffect()->numInputs;
        if (m_uiNumOutputs < (*it)->GetEffect()->numOutputs)
            m_uiNumOutputs = (*it)->GetEffect()->numOutputs;
    }

    float**             m_ppInputs = NULL;
    float**             m_ppOutputs = NULL;

    if (m_uiNumInputs > 0)
    {
        m_ppInputs = new float*[m_uiNumInputs];
        for (VstInt32 i = 0; i < m_uiNumInputs; i++)
        {
            m_ppInputs[i] = new float[kBlockSize];
            memset(m_ppInputs[i], 0, kBlockSize * sizeof(float));
        }
    }

    if (m_uiNumOutputs > 0)
    {
        m_ppOutputs = new float*[m_uiNumOutputs];
        for (VstInt32 i = 0; i < m_uiNumOutputs; i++)
        {
            m_ppOutputs[i] = new float[kBlockSize];
            memset(m_ppOutputs[i], 0, kBlockSize * sizeof(float));
        }
    }
    bool res;
    DWORD wait;
    if (ms_managedBuffer)
    {
        while (!stopProcessing)
        {
            EventManager::ResetEvent(EventManager::EVENT_PROCESSING_DONE);
            if (ms_iBufferRequestedSize > 0)
            {
                // samples from all plugins need to be taken and mixed together
                for (PluginInterfaceList::iterator it = pList->begin(); it != pList->end(); it++)
                {
                    (*it)->GetEffect()->processReplacing((*it)->GetEffect(), m_ppInputs, m_ppOutputs, ms_iBufferRequestedSize);

                    for (int bufferIndex = 0; bufferIndex < ms_managedBuffer->GetBufferCount(); bufferIndex++)
                    {
                        ms_managedBuffer->Write(bufferIndex, ms_iBufferRequestedSize, (int*)m_ppOutputs[bufferIndex]);
                    }
                }
                ms_SamplesProcessed += ms_iBufferRequestedSize;

            }
            EventManager::ResetEvent(EventManager::EVENT_DATA_WRITTEN);
            EventManager::SetEvent(EventManager::EVENT_PROCESSING_DONE);
            EventManager::WaitForEvent(EventManager::EVENT_DATA_WRITTEN);
        }
    }
    std::cout << "processing stopped" << std::endl;
    return 0;
}
