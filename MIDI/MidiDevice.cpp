#include "MidiDevice.h"
#include <Util/Logger.h>

using namespace eLibV2::MIDI;
using namespace eLibV2::Util;

#if defined(WIN32)
using namespace eLibV2::VST::Host;
#endif

#if defined(WIN32)
MidiDevice::MidiDevice(const PluginHost* hostThread) :
    m_pHostThread(hostThread),
    m_OpenedMidiIn(NULL),
    m_uiNumMidiInDevices(0)
#elif defined(__APPLE__)
MidiDevice::MidiDevice() :
    m_pMidiClient(NULL),
    m_pMidiInput(NULL)
#endif
{
    EnumerateMidiInDevices();
}

void MidiDevice::EnumerateMidiInDevices()
{
    std::stringstream ss;

#if defined(WIN32)
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
#elif defined(__APPLE__)
    m_uiNumMidiInDevices = MIDIGetNumberOfSources();
    ss << "midiIn devices: " << m_uiNumMidiInDevices << std::endl;

    for (ItemCount deviceIndex = 0; deviceIndex < m_uiNumMidiInDevices; deviceIndex++)
    {
        MIDIEndpointRef endpoint = MIDIGetSource(deviceIndex);
        CFStringRef result = CFSTR(""); // default

        MIDIObjectGetStringProperty(endpoint, kMIDIPropertyDisplayName, &result);
        // do something with the string
    }
    m_DeviceNames.push_back("default");
#elif defined(__linux__)
#endif
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
    ModuleLogger::print(LOG_CLASS_MIDI, "opening '%s'", GetDeviceName(deviceId).c_str());
    
#if defined(WIN32)
    MMRESULT mmr = midiInOpen(&m_OpenedMidiIn, deviceId, (DWORD_PTR)MidiDevice::CallbackFunction, (DWORD_PTR)m_pHostThread, CALLBACK_FUNCTION | MIDI_IO_STATUS);
    if (mmr != MMSYSERR_NOERROR)
        return false;

    ModuleLogger::print(LOG_CLASS_MIDI, "starting recording");
    mmr = midiInStart(m_OpenedMidiIn);
    if (mmr != MMSYSERR_NOERROR)
        return false;
#elif defined(__APPLE__)
    // Prepare MIDI Interface Client/Port for writing MIDI data:
    OSStatus status;
    
    if ((status = MIDIClientCreate(CFSTR("MIDIClient"), NULL, NULL, &m_pMidiClient)))
    {
        printf("Error trying to create MIDI Client structure: %d\n", (int)status);
        printf("%s\n", GetMacOSStatusErrorString(status));
        exit(status);
    }
    if ((status = MIDIInputPortCreate(m_pMidiClient, CFSTR("MIDIInput"), MidiDevice::CallbackFunction, NULL, &m_pMidiInput)))
    {
        printf("Error trying to create MIDI output port: %d\n", (int)status);
        printf("%s\n", GetMacOSStatusErrorString(status));
        exit(status);
    }
    
    for (ItemCount deviceIndex = 0; deviceIndex < m_uiNumMidiInDevices; deviceIndex++)
    {
        MIDIEndpointRef src = MIDIGetSource(deviceIndex);
        MIDIPortConnectSource(m_pMidiInput, src, NULL);
    }            
#elif defined(__linux__)
#endif
    return true;
}

void MidiDevice::CloseDevice()
{
#if defined(WIN32)
    if (m_OpenedMidiIn)
        midiInClose(m_OpenedMidiIn);
    m_OpenedMidiIn = NULL;
#endif
}

#if defined(WIN32)
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
#elif defined(__APPLE__)
void MidiDevice::CallbackFunction(const MIDIPacketList *packetList, void* readProcRefCon, void* srcConnRefCon)
{
    MIDIPacket *packet = (MIDIPacket*)packetList->packet;
    std::stringstream ss;

    if (packet != NULL)
    {
        for (UInt16 packetIndex = 0; packetIndex < packetList->numPackets; ++packetIndex)
        {
            ss.clear();
            int midistatus = packet->data[0] & 0xf0;
            int midichannel = packet->data[0] & 0x0f;
            
            switch (midistatus)
            {
                // note events (0x80 = note off / 0x90 = note on)
                case 0x80:
                case 0x90:
                {
                    int velocity = 0;
                    int note = packet->data[1];
                    
                    if (midistatus == 0x90)
                        velocity = packet->data[2];
                    
                    if (velocity > 0)
                        ss << "note on (" << (int)midichannel << "): note: " << (int)note << std::endl;
                    else
                        ss << "note off (" << (int)midichannel << "): note: " << (int)note << " velocity: " << (int)velocity << std::endl;
                    break;
                }
                    
                // control change
                case 0xb0:
                {
                    int controller = packet->data[1];
                    int value = packet->data[2];
                    ss << "control change (" << (int)midichannel << "): controller: " << (int)controller << " value: " << (int)value << std::endl;
                    break;
                }
            }
            ModuleLogger::print(LOG_CLASS_MIDI, ss.str().c_str());

            // get next packet in queue
            packet = MIDIPacketNext(packet);
        }            
    }
}
// do something on mac os
#elif defined(LINUX)
// do something o linux
#endif
