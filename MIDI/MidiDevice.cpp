#include "MidiDevice.h"

#ifdef WIN32

using namespace eLibV2::MIDI;
using namespace eLibV2::Host;

MidiDevice::MidiDevice(const PluginHost* hostThread)
    : m_pHostThread(hostThread),
    m_OpenedMidiIn(NULL),
    m_uiNumMidiInDevices(0)
{
    EnumerateMidiInDevices();
}

void MidiDevice::EnumerateMidiInDevices()
{
    m_uiNumMidiInDevices = midiInGetNumDevs();
    std::cout << "midiIn devices: " << m_uiNumMidiInDevices << std::endl;

    for (int deviceIndex = 0; deviceIndex < m_uiNumMidiInDevices; deviceIndex++)
    {
        MIDIINCAPS MidiInCaps;
        MMRESULT mmr = midiInGetDevCaps(deviceIndex, &MidiInCaps, sizeof(MIDIINCAPS));
        if (mmr != MMSYSERR_NOERROR)
            std::cout << "midiInGetDevCaps failed: mmr = " << std::hex << mmr << std::dec << std::endl;
        else
            m_DeviceNames.push_back(MidiInCaps.szPname);
    }
}

std::string MidiDevice::GetDeviceName(unsigned int deviceIndex)
{
    if (deviceIndex < m_DeviceNames.size())
        return m_DeviceNames[deviceIndex];
    else
        return std::string("not found");
}

bool MidiDevice::OpenDevice(int deviceId)
{
    std::cout << "opening " << m_DeviceNames[deviceId].c_str() << std::endl;
    MMRESULT mmr = midiInOpen(&m_OpenedMidiIn, deviceId, (DWORD_PTR)MidiDevice::CallbackFunction, (DWORD_PTR)m_pHostThread, CALLBACK_FUNCTION | MIDI_IO_STATUS);
    if (mmr != MMSYSERR_NOERROR)
        return false;

    std::cout << "starting recording" << std::endl;
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
    eLibV2::Host::PluginHost *host = (eLibV2::Host::PluginHost*)dwInstance;

    switch (wMsg)
    {
    case MIM_OPEN:
        std::cout << "MIM_OPEN received p1: " << dwParam1 << " p2: " << dwParam2 << std::endl;
        break;

    case MIM_CLOSE:
        std::cout << "MIM_CLOSE received p1: " << dwParam1 << " p2: " << dwParam2 << std::endl;
        break;

    case MIM_DATA:
    {
        int midiChannel = LOBYTE(LOWORD(dwParam1)) & 0x0f;
        int midiStatus = LOBYTE(LOWORD(dwParam1)) & 0xf0;
        int midiData1 = HIBYTE(LOWORD(dwParam1));
        int midiData2 = LOBYTE(HIWORD(dwParam1));

        std::cout << "MIM_DATA received channel: " << midiChannel << " status: " << midiStatus << " data1: " << midiData1 << " data2: " << midiData2 << std::endl;
        if (midiStatus == 0x90)
        {
            std::cout << "midi message note: " << midiData1 << " velocity: " << midiData2 << std::endl;
            host->InsertMidiEvent(midiChannel, midiStatus, midiData1, midiData2);
        }
        else if (midiStatus == 0xb0)
            std::cout << "control change event";
        else
            std::cout << "unknown midi event " << midiStatus;
        std::cout << std::endl;
        break;
    }

    case MIM_LONGDATA:
        std::cout << "MIM_LONGDATA received p1: " << dwParam1 << " p2: " << dwParam2 << std::endl;
        break;

    case MIM_MOREDATA:
        std::cout << "MIM_MOREDATA received p1: " << dwParam1 << " p2: " << dwParam2 << std::endl;
        break;
    }
}

#else
#error "MidiDevice implemented only on Windows"
#endif
