#include <VST/Host/VSTPluginHost.h>
#include <ASIO/AsioDevice.h>

using namespace eLibV2::VST::Host;
using namespace eLibV2::Util::Threads;

double PluginHost::ms_dSamplerate = kSampleRate;
double PluginHost::ms_dTempo = 140.0f;
UInt16 PluginHost::ms_uiBlocksize = kBlockSize;
UInt16 PluginHost::ms_uiTimeSignatureBeatsPerMeasure = 4;
UInt16 PluginHost::ms_uiTimeSignatureNoteValue = 4;
VstTimeInfo PluginHost::_VstTimeInfo;
bool PluginHost::ms_bTransportPlaying = false;
LARGE_INTEGER PluginHost::ms_liElapsedMicroseconds;
ManagedBuffer* PluginHost::ms_inputBuffer = NULL;
ManagedBuffer* PluginHost::ms_outputBuffer = NULL;
VstInt16 PluginHost::ms_iBufferRequestedSize = 0;
VstInt32 PluginHost::ms_SamplesProcessed = 0;

PluginHost::PluginHost()
{
    EventManager::RegisterEvent(EventManager::EVENT_PROCESSING_DONE);
}

PluginHost::~PluginHost()
{
    EventManager::UnregisterEvent(EventManager::EVENT_PROCESSING_DONE);
}

bool PluginHost::OpenPlugin(std::string fileName)
{
    ModuleLogger::print(LOG_CLASS_PLUGIN, "HOST> Load VST plugin '%s'", fileName.c_str());

    try
    {
        PluginInterface *plugin = new PluginInterface();

        if (plugin->Load(fileName, PluginHost::HostCallback))
        {
            std::string pluginID = plugin->GetPluginID();
            pluginID.append(1, (char)('0' + m_LoadedPlugins.size()));
            if (m_LoadedPlugins.count(pluginID) == 0)
                m_LoadedPlugins[pluginID] = plugin;
            else
            {
                ModuleLogger::print(LOG_CLASS_PLUGIN, "WARNING: duplicate plugin codes detected '%s'.", pluginID.c_str());
                plugin->Unload();
                return false;
            }

#if OUTPUT_PLUGIN_PROPERTIES == 1
            plugin->PrintProperties();
            plugin->PrintPrograms();
            plugin->PrintParameters();
            plugin->PrintCapabilities();
#endif

            // insert current plugin to relevant midi-channel
            if (plugin->CanReceiveMidi())
            {
                VstInt16 midiChannel = plugin->GetMidiChannel();
                m_PluginsForMIDIChannel[midiChannel].push_back(plugin);
            }
            return true;
        }
        else
        {
            ModuleLogger::print(LOG_CLASS_PLUGIN, "Failed to load VST Plugin library '%s'!", fileName.c_str());
            plugin->Unload();
        }
    }
    catch (std::bad_alloc)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "bad_alloc occured during plugin '%s' initialisation.", fileName.c_str());
    }
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

void PluginHost::UnloadAll()
{
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        (*it).second->Unload();
    m_LoadedPlugins.clear();
}

void PluginHost::StartAll()
{
    StartTimer();
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        (*it).second->Start();
}

void PluginHost::StopAll()
{
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        (*it).second->Stop();
    StopTimer();
}

void PluginHost::StartTimer()
{
    QueryPerformanceFrequency(&m_liFrequency);
    QueryPerformanceCounter(&m_liStartingTime);
    ms_bTransportPlaying = true;
}

void PluginHost::StopTimer()
{
    ms_bTransportPlaying = false;
    QueryPerformanceCounter(&m_liEndingTime);
    ms_liElapsedMicroseconds.QuadPart = m_liEndingTime.QuadPart - m_liStartingTime.QuadPart;
    ms_liElapsedMicroseconds.QuadPart *= 1000000;
    ms_liElapsedMicroseconds.QuadPart /= m_liFrequency.QuadPart;
}

VstInt32 PluginHost::CheckCanDo(char * canDo)
{
    return VstInt32();
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
//        "openFileSelector",
        "editFile",
//        "closeFileSelector"
    };

    VstInt32 res = 0;
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

PluginInterfaceList PluginHost::GetPluginsForMidiChannel(VstInt16 channel)
{
    PluginInterfaceList list;
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
    {
        PluginInterface *p = (*it).second;
        if ((p->CanReceiveMidi()) && (p->GetMidiChannel() == channel))
            list.push_back(p);
    }
    return list;
}

void PluginHost::InsertMidiEvent(VstInt16 channel, VstInt16 status, VstInt16 data1, VstInt16 data2)
{
    PluginInterfaceList list = m_PluginsForMIDIChannel[channel];
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
    std::stringstream ss;

    char pluginID[5] = "????";
    if (effect)
        PluginInterface::GetPluginStringFromLong(effect->uniqueID, pluginID);

    ss << "HOST> '" << pluginID << "': ";

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
        std::cout << "PLUG> HostCallback (opcode " << (VstInt16)opcode << ") index: " << (VstInt16)index << " value: " << (VstInt16)FromVstPtr<void>(value) << " ptr: " << ptr << " opt " << opt << std::endl;
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

    case __audioMasterPinConnectedDeprecated:
        // this is called by some plugins prior to vst2.4
//        ModuleLogger::print(LOG_CLASS_PLUGIN, "pin connected");
        break;

    case __audioMasterWantMidiDeprecated:
//        ModuleLogger::print(LOG_CLASS_PLUGIN, "want midi");
        break;

    case audioMasterGetTime:
//        ss << "requesting VstTimeInfo: " << std::hex << value << std::dec << std::endl;
//        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
//        ss.clear();

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
            {
                ss << "Plugin requested position in bars, but not PPQ" << std::endl;
                ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
            }

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
        {
            ss << "Current time in SMPTE format" << std::endl;
            ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
        }

        if (value & kVstClockValid)
        {
            ss << "Sample frames until next clock" << std::endl;
            ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
        }

        result = (VstIntPtr)&_VstTimeInfo;
        break;

    case audioMasterProcessEvents:
    {
        ss << "events from plugin received " << std::hex << ptr << std::dec << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());

        VstEvents* events = static_cast<VstEvents*>(ptr);
        for (VstInt32 eventIndex = 0; eventIndex < events->numEvents; eventIndex++)
        {
            switch ((events->events[eventIndex])->type)
            {
                case kVstMidiType:
                {
                    VstMidiEvent* event = (VstMidiEvent*)events->events[eventIndex];
                    char* midiData = event->midiData;
                    VstInt16 channel = midiData[0] & 0x0f;
                    VstInt16 status = midiData[0] & 0xf0;
                    VstInt16 note = midiData[1]; // &0x7f;
                    VstInt16 velocity = midiData[2]; // &0x7f;

                    ModuleLogger::print(LOG_CLASS_PLUGIN, "MIDI event received: %i %i %i %i", channel, status, note, velocity);
                }
                break;

                case kVstSysExType:
                    ModuleLogger::print(LOG_CLASS_PLUGIN, "Sysex event received:");
                    break;
            }
        }
    }
    break;

    case audioMasterIOChanged:
        // see AudioEffectX::ioChanged()
        ss << "number of inputs/outputs changing not supported" << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());

#if 0   // maybe not cool
        if (effect)
        {
            effect->dispatcher(effect, effMainsChanged, 0, 0, NULL, 0);
            effect->dispatcher(effect, effStopProcess, 0, 0, NULL, 0.0f);
            ss << "HOST> '" << pluginID << "': number of inputs/outputs after change: " << effect->numInputs << "/" << effect->numOutputs << std::endl;
            ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
            effect->dispatcher(effect, effMainsChanged, 0, 1, NULL, 0.0f);
            effect->dispatcher(effect, effStartProcess, 0, 0, NULL, 0.0f);
        }
#endif
        result = 0;
        break;

    case __audioMasterNeedIdleDeprecated:
        ModuleLogger::print(LOG_CLASS_PLUGIN, "need idle");
        break;

    case audioMasterSizeWindow:
        ss << "editor size changed. width: " << index << " height: " << value << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());

        result = 0;
        break;

    case audioMasterGetSampleRate:
        ss << "samplerate requested." << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());

        result = (VstInt16)GetSampleRate();
        break;

    case audioMasterGetBlockSize:
        ss << "blocksize requested." << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());

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
//        std::cout << "HOST> '" << pluginID << "': " << "vendor string requested" << std::endl;
        strncpy((char*)ptr, "e:fope media", kVstMaxVendorStrLen);
        result = 1;
        break;

    case audioMasterGetProductString:
//        std::cout << "HOST> '" << pluginID << "': " << "product string requested" << std::endl;
        strncpy((char*)ptr, "vst testhost", kVstMaxProductStrLen);
        result = 1;
        break;

    case audioMasterGetVendorVersion:
//        std::cout << "HOST> '" << pluginID << "': " << "product version requested" << std::endl;
        result = 1;
        break;

    case audioMasterVendorSpecific:
//        std::cout << "HOST> '" << pluginID << "': " << "vendor specific requested: " << std::hex << index << " value: " << value << " ptr: " << ptr << std::dec << " opt: " << opt << std::endl;
        result = 0;
        break;

    case audioMasterCanDo:
//        std::cout << "HOST> '" << pluginID << "': " << "request for master canDo '" << (char*)ptr << "'" << std::endl;
        result = CanHostDo((char*)ptr);
        break;

    case audioMasterGetLanguage:
//        std::cout << "HOST> '" << pluginID << "': " << "host language requested" << std::endl;
        result = kVstLangEnglish;
        break;

    case audioMasterUpdateDisplay:
//        std::cout << "HOST> '" << pluginID << "': " << "display updated" << std::endl;
        break;

    case audioMasterBeginEdit:
        ss << "editing parameter " << index << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
        break;

    case audioMasterEndEdit:
        ss << "ended editing parameter " << index << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
        break;

    case audioMasterOpenFileSelector:
        ss << "ptr: " << std::hex << ptr << std::dec << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
        result = 0;
        break;

    case audioMasterCloseFileSelector:
        ss << "ptr: " << std::hex << ptr << std::dec << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
        result = 0;
        break;

    default:
        ss << "opcode received: " << opcode << " index: " << index << " value: " << value << " ptr: " << std::hex << ptr << std::dec << " opt: " << opt << std::endl;
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
        break;
    }
    return result;
}

DWORD WINAPI PluginHost::ProcessReplacing(LPVOID lpParam)
{
    extern bool stopProcessing;
    static VstInt16 lastPreferredSize = 0;
    bool resetBufferSize = false;

    PluginInterfaceList *pList = reinterpret_cast<PluginInterfaceList*>(lpParam);

    std::vector<ProcessThreadParameters> processThreads;
    for (PluginInterfaceList::iterator it = pList->begin(); it != pList->end(); ++it)
    {
        // allocate memory for each of current plugin's inputs and outputs
        PluginInterface* currentPlugin = (*it);
        ProcessThreadParameters currentThread;

        currentThread.plugin = currentPlugin;
        currentThread.inputBuffer = ms_inputBuffer;
        currentThread.outputBuffer = ms_outputBuffer;
        processThreads.push_back(currentThread);
    }

    if (ms_inputBuffer && ms_outputBuffer)
    {
        while (!stopProcessing)
        {
            resetBufferSize = false;
            if (ms_iBufferRequestedSize > 0)
            {
                if (ms_iBufferRequestedSize != lastPreferredSize)
                {
                    lastPreferredSize = ms_iBufferRequestedSize;
                    resetBufferSize = true;
                }

                // samples from all plugins need to be taken and mixed together
                VstInt16 framesToProcess = ms_iBufferRequestedSize;
                if (ms_iBufferRequestedSize > kBlockSize)
                    framesToProcess = kBlockSize;

                // tell all plugins to process their inputs/outputs
                // this will be done in seperate threads for each plugin
                for (VstInt16 currentThreadIndex = 0; currentThreadIndex < processThreads.size(); currentThreadIndex++)
                {
                    PluginInterface* plugin = processThreads[currentThreadIndex].plugin;

                    if (resetBufferSize)
                    {
//                        plugin->Stop();
//                        plugin->Start();
                    }
                    plugin->SyncInputBuffers(processThreads[currentThreadIndex].inputBuffer, framesToProcess);
                    plugin->ProcessReplacing(framesToProcess);
                    plugin->SyncOutputBuffers(processThreads[currentThreadIndex].outputBuffer, framesToProcess);
                }
                ms_SamplesProcessed += framesToProcess;
            }
            EventManager::SetEvent(EventManager::EVENT_PROCESSING_DONE);
            if (!EventManager::WaitForEvent(EventManager::EVENT_DATA_WRITTEN, 500))
                break;

            EventManager::ResetEvent(EventManager::EVENT_DATA_WRITTEN);
        }
    }
    ModuleLogger::print(LOG_CLASS_PLUGIN, "processing stopped");
    return 0;
}
