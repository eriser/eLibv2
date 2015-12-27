#include <MIDI/BaseMidiDevice.h>

using namespace eLibV2::MIDI;

BaseMidiDevice::BaseMidiDevice() :
    m_uiNumMidiInDevices(0),
    m_pMidiHandler(NULL)
{
}

std::string BaseMidiDevice::GetDeviceName(UInt16 deviceIndex)
{
    if (deviceIndex < m_DeviceNames.size())
        return m_DeviceNames[deviceIndex];
    else
        return std::string("not found");
}
