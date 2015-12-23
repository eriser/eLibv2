#include <MIDI/BaseMidiDevice.h>
#include <MIDI/MidiDeviceFactory.h>
#include <Util/Logger.h>

using namespace eLibV2::MIDI;
using namespace eLibV2::Util;

#if defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#include <CoreMIDI/CoreMIDI.h>
#include <cstdio>
#elif defined(__linux__)

#else
#error "unsupported platform"
#endif

int main(void)
{
    BaseMidiDevice* midi;
    midi = MidiDeviceFactory::GetMidiDevice();
    
    ModuleLogger::enable();
    
    midi->OpenDevice(0);

#if defined(__APPLE__)
    CFRunLoopRef runLoop;
    runLoop = CFRunLoopGetCurrent();
    CFRunLoopRun();
#elif defined(__linux__)
    int *returnVal;
    
    pthread_join(midi.m_pMidiInputThread, (void**)&(returnVal));
    midi->CloseDevice();
#endif
    
    return 0;
}
