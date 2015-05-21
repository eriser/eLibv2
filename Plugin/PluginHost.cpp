#include <Plugin/PluginHost.h>

using namespace eLibV2::Host;

double PluginHost::ms_dSamplerate = kSampleRate;
double PluginHost::ms_dTempo = 140.0f;
unsigned int PluginHost::ms_uiBlocksize = kBlockSize;
unsigned int PluginHost::ms_uiTimeSignatureBeatsPerMeasure = 4;
unsigned int PluginHost::ms_uiTimeSignatureNoteValue = 4;
VstTimeInfo PluginHost::_VstTimeInfo;

PluginHost::PluginHost(): m_NumLoadedPlugins(0)
{
}

bool PluginHost::OpenPlugin(std::string fileName)
{
    std::cout << "HOST> Load VST plugin '" << fileName << "'" << std::endl;
    PluginInterface *plugin = new PluginInterface();

    if (plugin->Load(fileName, PluginHost::HostCallback))
    {
        m_LoadedPlugins[plugin->GetPluginID()] = plugin;

#if 0
        plugin->PrintProperties();
        plugin->PrintPrograms();
        plugin->PrintParameters();
        plugin->PrintCapabilities();
        plugin->ProcessReplacing();
#endif

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
}

void PluginHost::StopAll()
{
    for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        (*it).second->Stop();
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

PluginInterface *PluginHost::GetPluginByID(std::string pluginID)
{
    if (m_LoadedPlugins.count(pluginID) > 0)
        return m_LoadedPlugins[pluginID];
    else
        return NULL;
}

PluginInterface *PluginHost::GetPluginByIndex(const int pluginIndex)
{
    PluginInterface *res = NULL;
    if (pluginIndex < m_LoadedPlugins.size())
    {
        int currentPluginIndex = 0;
        for (PluginMap::iterator it = m_LoadedPlugins.begin(); it != m_LoadedPlugins.end(); it++)
        {
            if (currentPluginIndex == pluginIndex)
            {
                res = (*it).second;
                break;
            }
            currentPluginIndex++;
        }
    }
    return res;
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

    case 6:
        std::cout << "deprecated (audioMasterWantMidi)" << std::endl;
        break;

    case audioMasterGetTime:
        // These values are always valid
        _VstTimeInfo.samplePos = 0;
        _VstTimeInfo.sampleRate = kSampleRate;

        // Set flags for transport state
        _VstTimeInfo.flags = 0;
        _VstTimeInfo.flags |= kVstTransportChanged;
        _VstTimeInfo.flags |= kVstTransportPlaying;

        // Fill values based on other flags which may have been requested
        if (value & kVstNanosValid)
        {
            // It doesn't make sense to return this value, as the plugin may try to calculate
            // something based on the current system time. As we are running offline, anything
            // the plugin calculates here will probably be wrong given the way we are running.
            // However, for realtime mode, this flag should be implemented in that case.
        }

        if (value & kVstPpqPosValid)
        {
            // TODO: Move calculations to AudioClock
            double samplesPerBeat = (60.0 / GetTempo()) * GetSampleRate();
            // Musical time starts with 1, not 0
            _VstTimeInfo.ppqPos = (_VstTimeInfo.samplePos / samplesPerBeat) + 1.0;
            _VstTimeInfo.flags |= kVstPpqPosValid;
        }

        if (value & kVstTempoValid) {
            _VstTimeInfo.tempo = 140.0f;
            _VstTimeInfo.flags |= kVstTempoValid;
        }

        if (value & kVstBarsValid)
        {
            if (!(value & kVstPpqPosValid))
                std::cout << "Plugin requested position in bars, but not PPQ" << std::endl;

            // TODO: Move calculations to AudioClock
            double currentBarPos = floor(_VstTimeInfo.ppqPos / (double)GetTimeSignatureBeatsPerMeasure());
            _VstTimeInfo.barStartPos = currentBarPos * (double)GetTimeSignatureBeatsPerMeasure() + 1.0;
            std::cout << "Current bar is " << _VstTimeInfo.barStartPos << std::endl;
            _VstTimeInfo.flags |= kVstBarsValid;
        }

        if (value & kVstCyclePosValid)
            std::cout << "We don't support cycling, so this is always 0" << std::endl;

        if (value & kVstTimeSigValid) {
            _VstTimeInfo.timeSigNumerator = GetTimeSignatureBeatsPerMeasure();
            _VstTimeInfo.timeSigDenominator = GetTimeSignatureNoteValue();
            _VstTimeInfo.flags |= kVstTimeSigValid;
        }

        if (value & kVstSmpteValid)
            std::cout << "Current time in SMPTE format" << std::endl;

        if (value & kVstClockValid)
            std::cout << "Sample frames until next clock" << std::endl;

        result = (VstIntPtr)&_VstTimeInfo;
        std::cout << "requesting VstTimeInfo: " << std::hex << value << std::dec << std::endl;
        result = NULL;
        break;

    case audioMasterProcessEvents:
        std::cout << "events from plugin received " << std::hex << ptr << std::dec << std::endl;
        break;

    case audioMasterIOChanged:
        std::cout << "number of inputs/outputs changed" << std::endl;
        break;

    case audioMasterSizeWindow:
        std::cout << "editor size changed. width: " << index << " height: " << value << std::endl;
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
        result = kVstProcessLevelOffline;
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
        std::cout << "editor started editing" << std::endl;
        break;

    case audioMasterEndEdit:
        std::cout << "editor ended editing mode" << std::endl;
        break;

    case audioMasterOpenFileSelector:
        std::cout << "ptr: " << std::hex << ptr << std::dec << std::endl;
        break;

    case audioMasterCloseFileSelector:
        std::cout << "ptr: " << std::hex << ptr << std::dec << std::endl;
        break;

    default:
        std::cout << "opcode received: " << opcode << " index: " << index << " value: " << value << " ptr: " << std::hex << ptr << std::dec << " opt: " << opt << std::endl;
        break;
    }

    return result;
}
