#if defined(__APPLE__)

#include <CoreServices/CoreServices.h>
#include <CoreMIDI/CoreMIDI.h>
#include <cstdio>

#include <MIDI/MidiDevice.h>
#include <Util/Logger.h>

using namespace eLibV2::MIDI;
using namespace eLibV2::Util;

int main(void)
{
    MidiDevice midi;
    ModuleLogger::enable();
    
    midi.OpenDevice(0);
    
    CFRunLoopRef runLoop;
    runLoop = CFRunLoopGetCurrent();
    CFRunLoopRun();
    
    return 0;
}

#else

int main()
{
    return 0;
}

#endif