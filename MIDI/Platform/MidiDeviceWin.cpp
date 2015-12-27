#if defined(WIN32)

#include <MIDI/Platform/MidiDeviceWin.h>
#include <Util/Logger.h>

using namespace eLibV2::MIDI;
using namespace eLibV2::Util;
using namespace eLibV2::VST::Host;

MidiDeviceWin::MidiDeviceWin() :
    m_OpenedMidiIn(NULL)
{
    EnumerateMidiInDevices();
}

void MidiDeviceWin::EnumerateMidiInDevices()
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

bool MidiDeviceWin::OpenDevice(SInt16 deviceId)
{
    ModuleLogger::print(LOG_CLASS_MIDI, "opening '%s'", GetDeviceName(deviceId).c_str());

    MMRESULT mmr = midiInOpen(&m_OpenedMidiIn, deviceId, (DWORD_PTR)MidiDeviceWin::CallbackFunction, (DWORD_PTR)m_pMidiHandler, CALLBACK_FUNCTION | MIDI_IO_STATUS);
    if (mmr != MMSYSERR_NOERROR)
        return false;

    ModuleLogger::print(LOG_CLASS_MIDI, "starting recording");
    mmr = midiInStart(m_OpenedMidiIn);
    if (mmr != MMSYSERR_NOERROR)
        return false;

    return true;
}

void MidiDeviceWin::CloseDevice()
{
    if (m_OpenedMidiIn)
        midiInClose(m_OpenedMidiIn);
    m_OpenedMidiIn = NULL;
}

void CALLBACK MidiDeviceWin::CallbackFunction(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    std::stringstream ss;
    MidiEventHandler *midiHandler = (MidiEventHandler*)dwInstance;

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
            midiHandler->insertEvent(midiChannel, MidiEvent(midiData1, midiData2));
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

#endif
