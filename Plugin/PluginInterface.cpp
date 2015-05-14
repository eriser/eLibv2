#include "PluginInterface.h"

using namespace eLibV2::Host;

PluginInterface::PluginInterface()
    : m_pModule(NULL),
    m_pEffect(NULL),
    m_pHostCallback(NULL),
    m_fSamplerate(44100.0),
    m_uiBlocksize(512),
    m_uiMidiChannel(0),
    m_uiAudioChannel(0)
{
}

PluginInterface::~PluginInterface()
{
    if (m_pModule)
    {
#if _WIN32
        FreeLibrary((HMODULE)m_pModule);
#elif TARGET_API_MAC_CARBON
        CFBundleUnloadExecutable((CFBundleRef)module);
        CFRelease((CFBundleRef)module);
#endif
    }
}

bool PluginInterface::Load(const std::string fileName, audioMasterCallback callback)
{
    m_FileName = fileName;
    m_pHostCallback = callback;
#if _WIN32
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
    return false;
}

void PluginInterface::Unload()
{
    std::cout << "Plugin> Close effect..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effClose, 0, 0, 0, 0);
}

bool PluginInterface::CallPluginEntry()
{
    PluginEntryProc mainProc = NULL;

    std::cout << "Plugin> Searching for entry function..." << std::endl;
#if _WIN32
    mainProc = (PluginEntryProc)GetProcAddress((HMODULE)m_pModule, "VSTPluginMain");
    if (!mainProc)
        mainProc = (PluginEntryProc)GetProcAddress((HMODULE)m_pModule, "main");
#elif TARGET_API_MAC_CARBON
    mainProc = (PluginEntryProc)CFBundleGetFunctionPointerForName((CFBundleRef)m_pModule, CFSTR("VSTPluginMain"));
    if (!mainProc)
        mainProc = (PluginEntryProc)CFBundleGetFunctionPointerForName((CFBundleRef)m_pModule, CFSTR("main_macho"));
#endif

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
    return false;
}

void PluginInterface::Setup()
{
    std::cout << "Plugin> Setting up plugin..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effOpen, 0, 0, NULL, 0);
    m_pEffect->dispatcher(m_pEffect, effSetSampleRate, 0, 0, NULL, m_fSamplerate);
    m_pEffect->dispatcher(m_pEffect, effSetBlockSize, 0, m_uiBlocksize, NULL, 0);
    m_pEffect->dispatcher(m_pEffect, effSetEditKnobMode, 0, 2, NULL, 0);

    // get plugin id
    char pluginID[5] = { 0 };
    for (int i = 0; i < 4; i++)
        pluginID[i] = ((m_pEffect->uniqueID & (0xff << ((3 - i) * 8))) >> ((3 - i) * 8));
    m_PluginID.assign(pluginID);
}

void PluginInterface::Start()
{
    std::cout << "Plugin> " << m_PluginID << " Start requested..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effMainsChanged, 0, 1, 0, 0);
}

void PluginInterface::Stop()
{
    std::cout << "Plugin> " << m_PluginID << " Stop requested..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effMainsChanged, 0, 0, 0, 0);
}

void PluginInterface::SendMidi(int channel, int status, int data1, int data2)
{
    std::cout << "Plugin> " << m_PluginID << " Receiving Midi message..." << std::endl;
    VstEvents events;

    if (status == 0x90)
    {
        VstMidiEvent midiEvent = {};

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
    }
    m_pEffect->dispatcher(m_pEffect, effProcessEvents, 0, 0, &events, 0);
}

void PluginInterface::PrintProperties()
{
    std::cout << "Plugin> Gathering properties..." << std::endl;

    char effectName[256] = { 0 };
    char vendorString[256] = { 0 };
    char productString[256] = { 0 };

    m_pEffect->dispatcher(m_pEffect, effGetEffectName, 0, 0, effectName, 0);
    m_pEffect->dispatcher(m_pEffect, effGetVendorString, 0, 0, vendorString, 0);
    m_pEffect->dispatcher(m_pEffect, effGetProductString, 0, 0, productString, 0);

    std::cout << "Name: " << effectName << std::endl;
    std::cout << "Vendor: " << vendorString << std::endl;
    std::cout << "Product: " << productString << std::endl;

    std::cout << "numPrograms: " << m_pEffect->numPrograms << std::endl;
    std::cout << "numParams: " << m_pEffect->numParams << std::endl;
    std::cout << "numInputs: " << m_pEffect->numInputs << std::endl;
    std::cout << "numOutputs: " << m_pEffect->numOutputs << std::endl;
}

void PluginInterface::PrintPrograms()
{
    // Iterate programs...
    std::cout << "Plugin> Listing programs..." << std::endl;
    for (VstInt32 progIndex = 0; progIndex < m_pEffect->numPrograms; progIndex++)
    {
        char progName[256] = { 0 };
        if (!m_pEffect->dispatcher(m_pEffect, effGetProgramNameIndexed, progIndex, 0, progName, 0))
        {
            m_pEffect->dispatcher(m_pEffect, effSetProgram, 0, progIndex, 0, 0); // Note: old program not restored here!
            m_pEffect->dispatcher(m_pEffect, effGetProgramName, 0, 0, progName, 0);
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

        m_pEffect->dispatcher(m_pEffect, effGetParamName, paramIndex, 0, paramName, 0);
        m_pEffect->dispatcher(m_pEffect, effGetParamLabel, paramIndex, 0, paramLabel, 0);
        m_pEffect->dispatcher(m_pEffect, effGetParamDisplay, paramIndex, 0, paramDisplay, 0);
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
        VstInt32 result = (VstInt32)m_pEffect->dispatcher(m_pEffect, effCanDo, 0, 0, (void*)canDos[canDoIndex], 0);
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
            std::cout << "?????";
            break;
        }
        std::cout << std::endl;
    }
}

void PluginInterface::ProcessReplacing()
{
    float** inputs = 0;
    float** outputs = 0;
    VstInt32 numInputs = m_pEffect->numInputs;
    VstInt32 numOutputs = m_pEffect->numOutputs;

    if (numInputs > 0)
    {
        inputs = new float*[numInputs];
        for (VstInt32 i = 0; i < numInputs; i++)
        {
            inputs[i] = new float[m_uiBlocksize];
            memset(inputs[i], 0, m_uiBlocksize * sizeof(float));
        }
    }

    if (numOutputs > 0)
    {
        outputs = new float*[numOutputs];
        for (VstInt32 i = 0; i < numOutputs; i++)
        {
            outputs[i] = new float[m_uiBlocksize];
            memset(outputs[i], 0, m_uiBlocksize * sizeof(float));
        }
    }

    std::cout << "HOST> Resume effect..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effMainsChanged, 0, 1, 0, 0);

    // here the host should send a play command to gather some samples

    std::cout << "HOST> Process Replacing..." << std::endl;
    for (VstInt32 processCount = 0; processCount < kNumProcessCycles; processCount++)
    {
        m_pEffect->processReplacing(m_pEffect, inputs, outputs, m_uiBlocksize);
    }

    // here the host should send the gatheredd samples to the audio driver (asio?)

    std::cout << "HOST> Suspend effect..." << std::endl;
    m_pEffect->dispatcher(m_pEffect, effMainsChanged, 0, 0, 0, 0);

    if (numInputs > 0)
    {
        for (VstInt32 i = 0; i < numInputs; i++)
            delete[] inputs[i];
        delete[] inputs;
    }

    if (numOutputs > 0)
    {
        for (VstInt32 i = 0; i < numOutputs; i++)
            delete[] outputs[i];
        delete[] outputs;
    }
}
