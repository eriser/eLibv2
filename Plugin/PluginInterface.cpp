#include "PluginInterface.h"

using namespace eLibV2::Host;

PluginInterface::PluginInterface()
    : m_pModule(NULL),
    m_pEffect(NULL),
    m_pHostCallback(NULL),
    m_fSamplerate(44100.0),
    m_uiBlocksize(512),
    m_uiMidiChannel(0),
    m_uiAudioChannel(0),
    m_ePluginType(PluginType::PLUGIN_TYPE_UNSET),
    m_uiNumInputs(0),
    m_uiNumOutputs(0),
    m_ppInputs(NULL),
    m_ppOutputs(NULL),
    m_bCanReceiveMidi(false)
{
}

PluginInterface::~PluginInterface()
{

}

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
        if (CallPluginEntry())
        {
            Setup();
            return true;
        }
    }
    // in case of an error free the library
    Unload();
    return false;
}

void PluginInterface::Unload()
{
    std::cout << "Plugin> " << m_PluginID << " Close requested..." << std::endl;
    if (m_pEffect)
    {
        m_pEffect->dispatcher(m_pEffect, effClose, 0, 0, NULL, 0.0f);
        m_pEffect = NULL;
    }

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

bool PluginInterface::CallPluginEntry()
{
    PluginEntryProc mainProc = NULL;

    std::cout << "Plugin> Searching for entry function..." << std::endl;
#if WIN32
    mainProc = (PluginEntryProc)GetProcAddress((HMODULE)m_pModule, "VSTPluginMain");
    if (!mainProc)
    {
        OutputDebugString(m_FileName.c_str());
        OutputDebugString(": didn't find 'VSTPluginMain' in module. trying 'main' instead\n");
        mainProc = (PluginEntryProc)GetProcAddress((HMODULE)m_pModule, "main");
    }
#elif TARGET_API_MAC_CARBON
    mainProc = (PluginEntryProc)CFBundleGetFunctionPointerForName((CFBundleRef)m_pModule, CFSTR("VSTPluginMain"));
    if (!mainProc)
        mainProc = (PluginEntryProc)CFBundleGetFunctionPointerForName((CFBundleRef)m_pModule, CFSTR("main_macho"));
#endif

    try
    {
        if (mainProc)
        {
            std::cout << "Plugin> Creating effect instance..." << std::endl;
            m_pEffect = mainProc(m_pHostCallback);

            if (m_pEffect)
                return true;
            else
                std::cout << "Plugin> Failed to create effect instance!" << std::endl;
        }
        else
            std::cout << "Plugin> VST Plugin entry function not found! Tried 'VSTPluginMain' and 'main'." << std::endl;
    }
    catch (std::bad_alloc)
    {
        std::cout << "std::bad_alloc occured during plugin initialization" << std::endl;
    }
    return false;
}

void PluginInterface::Setup()
{
    std::cout << "Plugin> Setting up plugin..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effOpen, 0, 0, NULL, 0.0f);
    m_pEffect->dispatcher(m_pEffect, effSetSampleRate, 0, 0, NULL, m_fSamplerate);
    m_pEffect->dispatcher(m_pEffect, effSetBlockSize, 0, m_uiBlocksize, NULL, 0.0f);
    m_pEffect->dispatcher(m_pEffect, effSetEditKnobMode, 0, 2, NULL, 0.0f);

    // get plugin id
    char pluginID[5] = { 0 };
    GetPluginStringFromLong(m_pEffect->uniqueID, pluginID);
    m_PluginID.assign(pluginID);

    if (m_pEffect->flags & effFlagsIsSynth)
    {
        std::cout << "Plugin> This is an instrument plugin..." << std::endl;
        m_ePluginType = PluginType::PLUGIN_TYPE_INSTRUMENT;
    }
    else
    {
        std::cout << "Plugin> This is an effect plugin..." << std::endl;
        m_ePluginType = PluginType::PLUGIN_TYPE_EFFECT;
    }

    if (m_pEffect->dispatcher(m_pEffect, effGetPlugCategory, 0, 0, NULL, 0.0f) == kPlugCategShell)
    {
        std::cout << "Plugin> This is a shell plugin..." << std::endl;
        m_ePluginType = PluginType::PLUGIN_TYPE_SHELL;
    }

    const char *canDo = "receiveVstMidiEvent";
    if ((VstInt32)m_pEffect->dispatcher(m_pEffect, effCanDo, 0, 0, (void*)canDo, 0.0f) == 1)
        m_bCanReceiveMidi = true;

    SetupProcessingMemory();
}

void PluginInterface::SetupProcessingMemory()
{
    // get number of inputs/outputs -> these will NOT change during lifetime
    m_uiNumInputs = m_pEffect->numInputs;
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
        for (VstInt32 i = 0; i < m_uiNumInputs; i++)
            delete[] m_ppInputs[i];
        delete[] m_ppInputs;
    }

    if (m_uiNumOutputs > 0)
    {
        for (VstInt32 i = 0; i < m_uiNumOutputs; i++)
            delete[] m_ppOutputs[i];
        delete[] m_ppOutputs;
    }
}

void PluginInterface::Start()
{
    std::cout << "Plugin> " << m_PluginID << " Start requested..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effMainsChanged, 0, 1, NULL, 0.0f);
    m_pEffect->dispatcher(m_pEffect, effStartProcess, 0, 0, NULL, 0.0f);
}

void PluginInterface::Stop()
{
    std::cout << "Plugin> " << m_PluginID << " Stop requested..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effMainsChanged, 0, 0, NULL, 0);
    m_pEffect->dispatcher(m_pEffect, effStopProcess, 0, 0, NULL, 0.0f);
}

void PluginInterface::OpenEditor(void* window)
{
    std::cout << "Plugin> " << m_PluginID << " Editor Open requested..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effEditOpen, 0, 0, window, 0);
}

void PluginInterface::CloseEditor()
{
    std::cout << "Plugin> " << m_PluginID << " Editor Close requested..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effEditClose, 0, 0, NULL, 0);
}

void PluginInterface::IdleEditor()
{
    m_pEffect->dispatcher(m_pEffect, effEditIdle, 0, 0, NULL, 0.0f);
}

ERect* PluginInterface::GetEditorRect()
{
    ERect* eRect;
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
    std::cout << "Plugin> " << m_PluginID << " Receiving Midi message..." << std::endl;

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
    std::cout << "Plugin> Gathering properties..." << std::endl;

    char effectName[256] = { 0 };
    char vendorString[256] = { 0 };
    char productString[256] = { 0 };
    VstInt32 vendorVersion;

    m_pEffect->dispatcher(m_pEffect, effGetEffectName, 0, 0, effectName, 0.0f);
    m_pEffect->dispatcher(m_pEffect, effGetVendorString, 0, 0, vendorString, 0.0f);
    vendorVersion = m_pEffect->dispatcher(m_pEffect, effGetVendorVersion, 0, 0, NULL, 0.0f);
    
    m_pEffect->dispatcher(m_pEffect, effGetProductString, 0, 0, productString, 0.0f);

    std::cout << "Name: " << effectName << std::endl;
    std::cout << "Vendor: " << vendorString << std::endl;
    std::cout << "Product: " << productString << std::endl;

    std::cout << "numPrograms: " << m_pEffect->numPrograms << std::endl;
    std::cout << "numParams: " << m_pEffect->numParams << std::endl;
    std::cout << "numInputs: " << m_pEffect->numInputs << std::endl;
    std::cout << "numOutputs: " << m_pEffect->numOutputs << std::endl;

    std::cout << "InitialDelay: " << m_pEffect->initialDelay << " frames" << std::endl;

    if (m_ePluginType == PluginType::PLUGIN_TYPE_SHELL)
    {
        char nameBuffer[kVstMaxProductStrLen];
        std::cout << "Sub-plugins:" << std::endl;

        while (true)
        {
            memset(nameBuffer, 0, sizeof(nameBuffer));
            VstInt32 shellPluginId = (VstInt32)m_pEffect->dispatcher(m_pEffect, effShellGetNextPlugin, 0, 0, nameBuffer, 0.0f);

            if (shellPluginId == 0 || nameBuffer[0] == '0')
                break;
            else
            {
                char pluginID[5] = { 0 };
                GetPluginStringFromLong(shellPluginId, pluginID);
                std::cout << " '" << pluginID << "' (" << nameBuffer << ")" << std::endl;
            }
        }
    }
}

void PluginInterface::PrintPrograms()
{
    // Iterate programs...
    std::cout << "Plugin> Listing programs..." << std::endl;
    for (VstInt32 progIndex = 0; progIndex < m_pEffect->numPrograms; progIndex++)
    {
        char progName[256] = { 0 };
        if (!m_pEffect->dispatcher(m_pEffect, effGetProgramNameIndexed, progIndex, 0, progName, 0.0f))
        {
            m_pEffect->dispatcher(m_pEffect, effSetProgram, 0, progIndex, NULL, 0.0f); // Note: old program not restored here!
            m_pEffect->dispatcher(m_pEffect, effGetProgramName, 0, 0, progName, 0.0f);
        }
        std::cout << "Program " << (int)progIndex << ": " << progName << std::endl;
    }
}

void PluginInterface::PrintParameters()
{
    // Iterate parameters...
    std::cout << "Plugin> Listing parameters..." << std::endl;
    for (VstInt32 paramIndex = 0; paramIndex < m_pEffect->numParams; paramIndex++)
    {
        char paramName[256] = { 0 };
        char paramLabel[256] = { 0 };
        char paramDisplay[256] = { 0 };

        m_pEffect->dispatcher(m_pEffect, effGetParamName, paramIndex, 0, paramName, 0.0f);
        m_pEffect->dispatcher(m_pEffect, effGetParamLabel, paramIndex, 0, paramLabel, 0.0f);
        m_pEffect->dispatcher(m_pEffect, effGetParamDisplay, paramIndex, 0, paramDisplay, 0.0f);
        float value = m_pEffect->getParameter(m_pEffect, paramIndex);

        std::cout << "Param " << (int)paramIndex << ": " << paramName << "[" << paramDisplay << " " << paramLabel << "] (default: " << value << ")" << std::endl;
    }
}

void PluginInterface::PrintCapabilities()
{
    // Can-do nonsense...
    std::cout << "Plugin> Listing capabilities..." << std::endl;
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
        std::cout << "Can do " << canDos[canDoIndex] << "... ";
        VstInt32 result = (VstInt32)m_pEffect->dispatcher(m_pEffect, effCanDo, 0, 0, (void*)canDos[canDoIndex], 0.0f);
        switch (result)
        {
        case 0:
            std::cout << "don't know";
            break;

        case 1:
            std::cout << "yes";
            break;

        case -1:
            std::cout << "definitely not!";
            break;

        default:
            std::cout << "w00t?";
            break;
        }
        std::cout << std::endl;
    }
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
    m_pEffect->processReplacing(m_pEffect, m_ppInputs, m_ppOutputs, sampleFrames);
}
