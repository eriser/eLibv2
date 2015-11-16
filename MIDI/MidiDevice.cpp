#include "MidiDevice.h"

#ifdef WIN32

using namespace eLibV2::MIDI;
using namespace eLibV2::VST::Host;

MidiDevice::MidiDevice(const PluginHost* hostThread)
    : m_pHostThread(hostThread),
    m_OpenedMidiIn(NULL),
    m_uiNumMidiInDevices(0)
{
    EnumerateMidiInDevices();
}

void MidiDevice::EnumerateMidiInDevices()
{
    std::stringstream ss;

    m_uiNumMidiInDevices = midiInGetNumDevs();
    ss << "midiIn devices: " << m_uiNumMidiInDevices << std::endl;

    for (SInt16 deviceIndex = 0; deviceIndex < m_uiNumMidiInDevices; deviceIndex++)
    {
        MIDIINCAPS MidiInCaps;
        MMRESULT mmr = midiInGetDevCaps(deviceIndex, &MidiInCaps, sizeof(MIDIINCAPS));
        if (mmr != MMSYSERR_NOERROR)
            ss << "midiInGetDevCaps failed: mmr = " << std::hex << mmr << std::dec << std::endl;
        else
            m_DeviceNames.push_back(MidiInCaps.szPname);
    }
    ModuleLogger::print(LOG_CLASS_MIDI, ss.str().c_str());
}

std::string MidiDevice::GetDeviceName(UInt16 deviceIndex)
{
    if (deviceIndex < m_DeviceNames.size())
        return m_DeviceNames[deviceIndex];
    else
        return std::string("not found");
}

bool MidiDevice::OpenDevice(SInt16 deviceId)
{
    ModuleLogger::print(LOG_CLASS_MIDI, "opening '%s'", m_DeviceNames[deviceId].c_str());
    MMRESULT mmr = midiInOpen(&m_OpenedMidiIn, deviceId, (DWORD_PTR)MidiDevice::CallbackFunction, (DWORD_PTR)m_pHostThread, CALLBACK_FUNCTION | MIDI_IO_STATUS);
    if (mmr != MMSYSERR_NOERROR)
        return false;

    ModuleLogger::print(LOG_CLASS_MIDI, "starting recording");
    mmr = midiInStart(m_OpenedMidiIn);
    if (mmr != MMSYSERR_NOERROR)
        return false;

    return true;
}

void MidiDevice::CloseDevice()
{
    if (m_OpenedMidiIn)
        midiInClose(m_OpenedMidiIn);
    m_OpenedMidiIn = NULL;
}

void CALLBACK MidiDevice::CallbackFunction(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    std::stringstream ss;
    eLibV2::VST::Host::PluginHost *host = (eLibV2::VST::Host::PluginHost*)dwInstance;

    switch (wMsg)
    {
    case MIM_OPEN:
        ss << "MIM_OPEN received p1: " << dwParam1 << " p2: " << dwParam2 << std::endl;
        break;

    case MIM_CLOSE:
        ss << "MIM_CLOSE received p1: " << dwParam1 << " p2: " << dwParam2 << std::endl;
        break;

    case MIM_DATA:
    {
        SInt16 midiChannel = LOBYTE(LOWORD(dwParam1)) & 0x0f;
        SInt16 midiStatus = LOBYTE(LOWORD(dwParam1)) & 0xf0;
        SInt16 midiData1 = HIBYTE(LOWORD(dwParam1));
        SInt16 midiData2 = LOBYTE(HIWORD(dwParam1));

        ss << "MIM_DATA received channel: " << midiChannel << " status: " << midiStatus << " data1: " << midiData1 << " data2: " << midiData2;
        if (midiStatus == 0x90)
        {
            ss << "midi message note: " << midiData1 << " velocity: " << midiData2;
            host->InsertMidiEvent(midiChannel, midiStatus, midiData1, midiData2);
        }
        else if (midiStatus == 0xb0)
            ss << "control change event";
        else
            ss << "unknown midi event " << midiStatus;
        ss << std::endl;
        break;
    }

    case MIM_LONGDATA:
        ss << "MIM_LONGDATA received p1: " << dwParam1 << " p2: " << dwParam2 << std::endl;
        break;

    case MIM_MOREDATA:
        ss << "MIM_MOREDATA received p1: " << dwParam1 << " p2: " << dwParam2 << std::endl;
        break;
    }
    ModuleLogger::print(LOG_CLASS_MIDI, ss.str().c_str());
}

#else
//#error "MidiDevice currently implemented only on Windows"
#endif
