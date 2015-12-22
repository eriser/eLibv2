#if defined(__linux__)

#include <MIDI/Platform/MidiDeviceLinux.h>
#include <Util/Logger.h>

using namespace eLibV2::MIDI;
using namespace eLibV2::Util;

MidiDeviceLinux::MidiDeviceLinux() :
//    m_pMidiInputThread(0),
    m_pMidiInput(NULL)
{
    EnumerateMidiInDevices();
}

void MidiDeviceLinux::EnumerateMidiInDevices()
{
    std::stringstream ss;

    m_uiNumMidiInDevices = 1;
    m_DeviceNames.push_back("hw:1,0,0");

    ModuleLogger::print(LOG_CLASS_MIDI, ss.str().c_str());
}

bool MidiDeviceLinux::OpenDevice(SInt16 deviceId)
{
    ModuleLogger::print(LOG_CLASS_MIDI, "opening '%s'", GetDeviceName(deviceId).c_str());
    
    int status;
    int mode = 0;

    if ((status = snd_rawmidi_open(&m_pMidiInput, NULL, GetDeviceName(deviceId).c_str(), mode)) < 0)
    {
        ModuleLogger::print(LOG_CLASS_MIDI, "Problem opening MIDI input: %s", snd_strerror(status));
        return false;
    }

    status = pthread_create(&m_pMidiInputThread, NULL, MidiDeviceLinux::CallbackFunction, m_pMidiInput);
    if (status == -1)
    {
        ModuleLogger::print(LOG_CLASS_MIDI, "Unable to create MIDI input thread.");
        return false;
    }
    return true;
}

void MidiDeviceLinux::CloseDevice()
{
    snd_rawmidi_close(m_pMidiInput);
    m_pMidiInput = NULL;
}

void *MidiDeviceLinux::CallbackFunction(void *arg)
{
    std::stringstream ss;

    // this is the parameter passed via last argument of pthread_create():
    snd_rawmidi_t* midiin = (snd_rawmidi_t*)arg;
    snd_rawmidi_status_t* midistatus;
    char buffer[256];
    int status;

    status = snd_rawmidi_status_malloc(&midistatus);

    while (1)
    {
        ss.clear();

        if (midiin == NULL)
            break;

        status = snd_rawmidi_status(midiin, midistatus);
        size_t avail = snd_rawmidi_status_get_avail(midistatus);

//        if (avail > 0)
        {
            ModuleLogger::print(LOG_CLASS_MIDI, "%d bytes available", avail);
            if ((status = snd_rawmidi_read(midiin, buffer, 9)) < 0)
                ModuleLogger::print(LOG_CLASS_MIDI, "Problem reading MIDI input: %s", snd_strerror(status));

            status = snd_rawmidi_drain(midiin);

            int midistatus = buffer[0] & 0xf0;
            int midichannel = buffer[0] & 0x0f;

            switch (midistatus)
            {
                // note events (0x80 = note off / 0x90 = note on)
                case 0x80:
                case 0x90:
                {
                    int velocity = 0;
                    int note = buffer[1];
                    
                    if (midistatus == 0x90)
                        velocity = buffer[2];
                    
                    if (velocity > 0)
                        ss << "note on (" << (int)midichannel << "): note: " << (int)note << std::endl;
                    else
                        ss << "note off (" << (int)midichannel << "): note: " << (int)note << " velocity: " << (int)velocity << std::endl;
                    break;
                }
                    
                // control change
                case 0xb0:
                {
                    int controller = buffer[1];
                    int value = buffer[2];
                    ss << "control change (" << (int)midichannel << "): controller: " << (int)controller << " value: " << (int)value << std::endl;
                    break;
                }
            }
            ModuleLogger::print(LOG_CLASS_MIDI, ss.str().c_str());
            
        }
    }
    snd_rawmidi_status_free(midistatus);
    return NULL;
}

#endif
