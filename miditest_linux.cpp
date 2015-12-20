#if defined(__linux__)

#include <MIDI/MidiDevice.h>
#include <Util/Logger.h>

using namespace eLibV2::MIDI;
using namespace eLibV2::Util;

int main(int argc, char *argv[])
{
    MidiDevice midi;

    ModuleLogger::enable();

    midi.OpenDevice(0);
    int *returnVal;

    pthread_join(midi.m_pMidiInputThread, (void**)&(returnVal));
    midi.CloseDevice();

    return 0;
}

#else

int main()
{
    return 0;
}

#endif