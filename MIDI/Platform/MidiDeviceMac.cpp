#if defined(__APPLE__)

#include <MIDI/Platform/MidiDeviceMac.h>
#include <Util/Logger.h>

using namespace eLibV2::MIDI;
using namespace eLibV2::Util;

MidiDeviceMac::MidiDeviceMac() :
    m_pMidiClient(NULL),
    m_pMidiInput(NULL)
{
    EnumerateMidiInDevices();
}

void MidiDeviceMac::EnumerateMidiInDevices()
{
    std::stringstream ss;

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

    ModuleLogger::print(LOG_CLASS_MIDI, ss.str().c_str());
}

bool MidiDeviceMac::OpenDevice(SInt16 deviceId)
{
    ModuleLogger::print(LOG_CLASS_MIDI, "opening '%s'", GetDeviceName(deviceId).c_str());

    // Prepare MIDI Interface Client/Port for writing MIDI data:
    OSStatus status;

    if ((status = MIDIClientCreate(CFSTR("MIDIClient"), NULL, NULL, &m_pMidiClient)))
    {
        ModuleLogger::print(LOG_CLASS_MIDI, "Error trying to create MIDI Client structure: %d\n", (int)status);
        ModuleLogger::print(LOG_CLASS_MIDI, "%s\n", GetMacOSStatusErrorString(status));
        return false;
    }
    if ((status = MIDIInputPortCreate(m_pMidiClient, CFSTR("MIDIInput"), MidiDeviceMac::CallbackFunction, NULL, &m_pMidiInput)))
    {
        ModuleLogger::print(LOG_CLASS_MIDI, "Error trying to create MIDI output port: %d\n", (int)status);
        ModuleLogger::print(LOG_CLASS_MIDI, "%s\n", GetMacOSStatusErrorString(status));
        return false;
    }

    for (ItemCount deviceIndex = 0; deviceIndex < m_uiNumMidiInDevices; deviceIndex++)
    {
        MIDIEndpointRef src = MIDIGetSource(deviceIndex);
        MIDIPortConnectSource(m_pMidiInput, src, NULL);
    }
    return true;
}

void MidiDeviceMac::CloseDevice()
{
}

void MidiDeviceMac::CallbackFunction(const MIDIPacketList *packetList, void* readProcRefCon, void* srcConnRefCon)
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

#endif
