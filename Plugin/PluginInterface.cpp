#include "PluginInterface.h"

using namespace eLibV2::Host;

bool PluginInterface::Load(const std::string fileName, audioMasterCallback callback)
{
    m_FileName = fileName;
    m_pHostCallback = callback;
#if WIN32
    m_pModule = LoadLibrary(m_FileName.c_str());
#elif TARGET_API_MAC_CARBON
    CFStringRef fileNameString = CFStringCreateWithCString(NULL, fileName.c_str(), kCFStringEncodingUTF8);
    if (fileNameString == 0)
        return false;
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, fileNameString, kCFURLPOSIXPathStyle, false);
    CFRelease(fileNameString);
    if (url == 0)
        return false;
    module = CFBundleCreate(NULL, url);
    CFRelease(url);
    if (module && CFBundleLoadExecutable((CFBundleRef)module) == false)
        return false;
#endif
    if (m_pModule)
    {
        if (AttachHostCallback())
        {
            SetupProcessingMemory();
            SetupPlugin();
            Open();
            return true;
        }
    }
    return false;
}

void PluginInterface::Unload()
{
    Close();
    FreeProcessingMemory();

    if (m_pModule)
    {
#if WIN32
        FreeLibrary((HMODULE)m_pModule);
#elif TARGET_API_MAC_CARBON
        CFBundleUnloadExecutable((CFBundleRef)module);
        CFRelease((CFBundleRef)module);
#endif
        m_pHostCallback = NULL;
        m_pModule = NULL;
    }
}

bool PluginInterface::AttachHostCallback()
{
    PluginEntryProc mainProc = NULL;

    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Searching for entry function...");
#if WIN32
    try
    {
        mainProc = (PluginEntryProc)GetProcAddress((HMODULE)m_pModule, "VSTPluginMain");
        if (!mainProc)
        {
            ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> VSTPluginMain not found. Trying main instead.");
            mainProc = (PluginEntryProc)GetProcAddress((HMODULE)m_pModule, "main");
        }

#elif TARGET_API_MAC_CARBON
        mainProc = (PluginEntryProc)CFBundleGetFunctionPointerForName((CFBundleRef)m_pModule, CFSTR("VSTPluginMain"));
        if (!mainProc)
            mainProc = (PluginEntryProc)CFBundleGetFunctionPointerForName((CFBundleRef)m_pModule, CFSTR("main_macho"));
#endif

        if (mainProc)
        {
            ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Creating effect instance...");
            m_pEffect = mainProc(m_pHostCallback);

            if (m_pEffect)
                return true;
            else
                ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Failed to create effect instance!");
        }
        else
            ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: VST Plugin entry function not found! Tried 'VSTPluginMain' and 'main'.", m_FileName.c_str());
    }
    catch (std::bad_alloc)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: MEMORY ERROR: std::bad_alloc occured during plugin initialization", m_FileName.c_str());
    }
    catch (...)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Exception: %s: ", m_FileName.c_str());
    }
    return false;
}

void PluginInterface::SetupPlugin()
{
    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Setting up plugin...");
    m_pEffect->dispatcher(m_pEffect, effSetSampleRate, 0, 0, NULL, m_fSamplerate);
    SetBlocksize(m_uiBlocksize);
    m_pEffect->dispatcher(m_pEffect, effSetEditKnobMode, 0, 2, NULL, 0.0f);
    m_uiVstVersion = (unsigned int)m_pEffect->dispatcher(m_pEffect, effGetVstVersion, 0, 0, NULL, 0.0f);

    // get plugin id
    char pluginID[5] = { 0 };
    GetPluginStringFromLong(m_pEffect->uniqueID, pluginID);
    m_sPluginID.assign(pluginID);

    if (m_pEffect->flags & effFlagsIsSynth)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> This is an instrument plugin...");
        m_ePluginType = PluginType::PLUGIN_TYPE_INSTRUMENT;
    }
    else
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> This is an effect plugin...");
        m_ePluginType = PluginType::PLUGIN_TYPE_EFFECT;
    }

    if (m_pEffect->dispatcher(m_pEffect, effGetPlugCategory, 0, 0, NULL, 0.0f) == kPlugCategShell)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> This is a shell plugin...");
        m_ePluginType = PluginType::PLUGIN_TYPE_SHELL;
    }

    if (m_pEffect->flags & effFlagsHasEditor)
        m_bHasEditor = true;

    if (m_pEffect->flags & effFlagsCanReplacing)
        m_bCanReplacing = true;

    const char *canDo = "receiveVstMidiEvent";
    if ((VstInt32)m_pEffect->dispatcher(m_pEffect, effCanDo, 0, 0, (void*)canDo, 0.0f) == 1)
        m_bCanReceiveMidi = true;

    m_uiNumPrograms = m_pEffect->numPrograms;
}

void PluginInterface::SetupProcessingMemory()
{
    // get number of inputs/outputs -> hopefully these will NOT change during lifetime
    m_uiNumInputs = m_pEffect->numInputs; // > 0 ? m_pEffect->numInputs : 2;
    m_uiNumOutputs = m_pEffect->numOutputs;

    if (m_uiNumInputs > 0)
    {
        m_ppInputs = new float*[m_uiNumInputs];
        for (VstInt32 i = 0; i < m_uiNumInputs; i++)
        {
            m_ppInputs[i] = new float[m_uiBlocksize];
            memset(m_ppInputs[i], 0, m_uiBlocksize * sizeof(float));
        }
    }

    if (m_uiNumOutputs > 0)
    {
        m_ppOutputs = new float*[m_uiNumOutputs];
        for (VstInt32 i = 0; i < m_uiNumOutputs; i++)
        {
            m_ppOutputs[i] = new float[m_uiBlocksize];
            memset(m_ppOutputs[i], 0, m_uiBlocksize * sizeof(float));
        }
    }
}

void PluginInterface::FreeProcessingMemory()
{
    if (m_uiNumInputs > 0)
    {
        if (m_ppInputs)
        {
            for (VstInt32 i = 0; i < m_uiNumInputs; i++)
                delete[] m_ppInputs[i];
            delete[] m_ppInputs;
            m_ppInputs = NULL;
        }
    }

    if (m_uiNumOutputs > 0)
    {
        if (m_ppOutputs)
        {
            for (VstInt32 i = 0; i < m_uiNumOutputs; i++)
                delete[] m_ppOutputs[i];
            delete[] m_ppOutputs;
            m_ppOutputs = NULL;
        }
    }
}

void PluginInterface::SetBlocksize(VstInt32 blocksize)
{
    m_uiBlocksize = blocksize;
    m_pEffect->dispatcher(m_pEffect, effSetBlockSize, 0, m_uiBlocksize, NULL, 0.0f);
}

void PluginInterface::Open()
{
    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: Open requested...", m_sPluginID.c_str());
    if (m_pEffect)
    {
        // call open -> empty implementation by default?
        m_pEffect->dispatcher(m_pEffect, effOpen, 0, 0, NULL, 0.0f);
    }
}

void PluginInterface::Close()
{
    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: Close requested...", m_sPluginID.c_str());
    if (m_pEffect)
    {
        // call close -> empty implementation by default?
        m_pEffect->dispatcher(m_pEffect, effClose, 0, 0, NULL, 0.0f);
//        m_pEffect = NULL;
    }
}

void PluginInterface::Start()
{
    try
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: Start requested...", m_sPluginID.c_str());
        // call resume
        m_pEffect->dispatcher(m_pEffect, effMainsChanged, 0, 1, NULL, 0.0f);
        //    m_pEffect->dispatcher(m_pEffect, effStartProcess, 0, 0, NULL, 0.0f);

        // emulate behaviour of FLStudio after calling resume to get plugin ready
        const char *canDo = "receiveVstMidiEvent";
        if ((VstInt32)m_pEffect->dispatcher(m_pEffect, effCanDo, 0, 0, (void*)canDo, 0.0f) == 1)
            m_bCanReceiveMidi = true;

        m_bPluginRunning = true;
    }
    catch (const std::exception& e)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> exception in Start(): %s", e.what());
    }
    catch (const std::string& s)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> exception in Start(): %s", s.c_str());
    }
    catch (...)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> exception in Start()");
    }
}

void PluginInterface::Stop()
{
    try
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: Stop requested...", m_sPluginID.c_str());
        m_bPluginRunning = false;
        // call suspend
        m_pEffect->dispatcher(m_pEffect, effMainsChanged, 0, 0, NULL, 0.0f);
        //    m_pEffect->dispatcher(m_pEffect, effStopProcess, 0, 0, NULL, 0.0f);
    }
    catch (std::exception e)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> exception in Stop(): %s", e.what());
    }
    catch (const std::string& s)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> exception in Stop(): %s", s.c_str());
    }
    catch (...)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> exception in Stop()");
    }
}

void PluginInterface::OpenEditor(void* window)
{
    if (m_bHasEditor)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: Editor Open requested...", m_sPluginID.c_str());
        m_pEffect->dispatcher(m_pEffect, effEditOpen, 0, 0, window, 0);
    }
}

void PluginInterface::CloseEditor()
{
    if (m_bHasEditor)
    {
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: Editor Close requested...", m_sPluginID.c_str());
        m_pEffect->dispatcher(m_pEffect, effEditClose, 0, 0, NULL, 0);
    }
}

void PluginInterface::IdleEditor()
{
    if (m_bHasEditor)
        m_pEffect->dispatcher(m_pEffect, effEditIdle, 0, 0, NULL, 0.0f);
}

ERect* PluginInterface::GetEditorRect()
{
    ERect* eRect = NULL;
    if (m_bHasEditor)
        m_pEffect->dispatcher(m_pEffect, effEditGetRect, 0, 0, &eRect, 0);
    return eRect;
}

std::string PluginInterface::GetEffectName()
{
    char effectName[kVstMaxEffectNameLen + 1] = { 0 };
    m_pEffect->dispatcher(m_pEffect, effGetEffectName, 0, 0, &effectName, 0);
    return std::string(effectName);
}

void PluginInterface::SendMidi(int channel, int status, int data1, int data2)
{
    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> %s: Receiving Midi message...", m_sPluginID.c_str());

    if (status == 0x90)
    {
        // MIDI Event

        midiEvent.byteSize = sizeof(VstMidiEvent);
        midiEvent.type = kVstMidiType;
        midiEvent.midiData[0] = (data2 == 0x00) ? 0x80 : 0x90;
        midiEvent.midiData[0] += channel;
        midiEvent.midiData[1] = data1;
        midiEvent.midiData[2] = data2;

        events.numEvents = 1;
        events.events[0] = (VstEvent*)&midiEvent;
    }
    else if (status == 0xb0)
    {
        // Sysex Event
    }
    m_pEffect->dispatcher(m_pEffect, effProcessEvents, 0, 0, &events, 0.0f);
}

void PluginInterface::PrintProperties()
{
    std::stringstream ss;

    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Gathering properties...");

    char effectName[256] = { 0 };
    char vendorString[256] = { 0 };
    char productString[256] = { 0 };
    VstInt32 vendorVersion;

    m_pEffect->dispatcher(m_pEffect, effGetEffectName, 0, 0, effectName, 0.0f);
    m_pEffect->dispatcher(m_pEffect, effGetVendorString, 0, 0, vendorString, 0.0f);
    vendorVersion = m_pEffect->dispatcher(m_pEffect, effGetVendorVersion, 0, 0, NULL, 0.0f);
    
    m_pEffect->dispatcher(m_pEffect, effGetProductString, 0, 0, productString, 0.0f);

    ss << "Name: " << effectName << std::endl;
    ss << "Vendor: " << vendorString << std::endl;
    ss << "Product: " << productString << std::endl;

    ss << "numPrograms: " << m_pEffect->numPrograms << std::endl;
    ss << "numParams: " << m_pEffect->numParams << std::endl;
    ss << "numInputs: " << m_pEffect->numInputs << std::endl;
    ss << "numOutputs: " << m_pEffect->numOutputs << std::endl;

    ss << "InitialDelay: " << m_pEffect->initialDelay << " frames" << std::endl;
    ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());

    if (m_ePluginType == PluginType::PLUGIN_TYPE_SHELL)
    {
        char nameBuffer[kVstMaxProductStrLen];
        ss << "Sub-plugins:" << std::endl;

        while (true)
        {
            memset(nameBuffer, 0, sizeof(nameBuffer));
            VstInt32 shellPluginId = (VstInt32)m_pEffect->dispatcher(m_pEffect, effShellGetNextPlugin, 0, 0, nameBuffer, 0.0f);

            if (shellPluginId == 0 || nameBuffer[0] == '\0')
                break;
            else
            {
                char pluginID[5] = { 0 };
                GetPluginStringFromLong(shellPluginId, pluginID);
                ss << " '" << pluginID << "' (" << nameBuffer << ")" << std::endl;
            }
        }
        ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
    }
}

void PluginInterface::PrintPrograms()
{
    // Iterate programs...
    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Listing programs...");
    for (VstInt32 progIndex = 0; progIndex < m_pEffect->numPrograms; progIndex++)
    {
        char progName[256] = { 0 };
        GetProgramName(progIndex, progName);
        ModuleLogger::print(LOG_CLASS_PLUGIN, "Program %li: %s", progIndex, progName);
    }
}

void PluginInterface::GetProgramName(VstInt32 progIndex, char* progName)
{
    if (!m_pEffect->dispatcher(m_pEffect, effGetProgramNameIndexed, progIndex, 0, progName, 0.0f))
    {
        SetProgram(progIndex);
        m_pEffect->dispatcher(m_pEffect, effGetProgramName, 0, 0, progName, 0.0f);
    }
}

void PluginInterface::SetProgram(VstInt32 progIndex)
{
    m_pEffect->dispatcher(m_pEffect, effSetProgram, 0, progIndex, NULL, 0.0f); // Note: old program not restored here!
}

void PluginInterface::PrintParameters()
{
    // Iterate parameters...
    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Listing parameters...");
    for (VstInt32 paramIndex = 0; paramIndex < m_pEffect->numParams; paramIndex++)
    {
        char paramName[256] = { 0 };
        char paramLabel[256] = { 0 };
        char paramDisplay[256] = { 0 };

        m_pEffect->dispatcher(m_pEffect, effGetParamName, paramIndex, 0, paramName, 0.0f);
        m_pEffect->dispatcher(m_pEffect, effGetParamLabel, paramIndex, 0, paramLabel, 0.0f);
        m_pEffect->dispatcher(m_pEffect, effGetParamDisplay, paramIndex, 0, paramDisplay, 0.0f);
        float value = m_pEffect->getParameter(m_pEffect, paramIndex);

        ModuleLogger::print(LOG_CLASS_PLUGIN, "Param %li: %s ['%s' '%s'] (default: %f)", paramIndex, paramName, paramDisplay, paramLabel, value);
    }
}

void PluginInterface::PrintCapabilities()
{
    // Can-do nonsense...
    std::stringstream ss;
    ModuleLogger::print(LOG_CLASS_PLUGIN, "Plugin> Listing capabilities...");
    static const char* canDos[] =
    {
        "sendVstEvents",
        "sendVstMidiEvent",
        "receiveVstEvents",
        "receiveVstMidiEvent",
        "receiveVstTimeInfo",
        "offline",
        "midiProgramNames",
        "bypass"
    };

    for (VstInt32 canDoIndex = 0; canDoIndex < sizeof(canDos) / sizeof(canDos[0]); canDoIndex++)
    {
        ss << "Can do " << canDos[canDoIndex] << "... ";
        VstInt32 result = (VstInt32)m_pEffect->dispatcher(m_pEffect, effCanDo, 0, 0, (void*)canDos[canDoIndex], 0.0f);
        switch (result)
        {
        case 0:
            ss << "don't know";
            break;

        case 1:
            ss << "yes";
            break;

        case -1:
            ss << "definitely not!";
            break;

        default:
            ss << "w00t?";
            break;
        }
        ss << std::endl;
    }
    ModuleLogger::print(LOG_CLASS_PLUGIN, ss.str().c_str());
}

void PluginInterface::SyncInputBuffers(ManagedBuffer* managedBuffer, VstInt32 dataSize)
{
    if (managedBuffer)
    {
        for (int bufferIndex = 0; bufferIndex < m_uiNumInputs; ++bufferIndex)
            managedBuffer->Read(bufferIndex, dataSize, (int*)m_ppInputs[bufferIndex]);
    }
}

void PluginInterface::SyncOutputBuffers(ManagedBuffer* managedBuffer, VstInt32 dataSize)
{
    if (managedBuffer)
    {
        for (int bufferIndex = 0; bufferIndex < m_uiNumOutputs; ++bufferIndex)
            managedBuffer->Write(bufferIndex, dataSize, (int*)m_ppOutputs[bufferIndex]);
    }
}

void PluginInterface::ProcessReplacing(VstInt32 sampleFrames)
{
    if (m_bCanReplacing && m_bPluginRunning)
        m_pEffect->processReplacing(m_pEffect, m_ppInputs, m_ppOutputs, sampleFrames);
}
