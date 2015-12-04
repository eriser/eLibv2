//
// Programmer:	Craig Stuart Sapp
// Date:	Wed Jun 10 12:40:22 PDT 2009
// Filename:	testin2.c
// Syntax:	C; Apple OSX CoreMIDI
// $Smake:	gcc -o %b %f -framework CoreMIDI -framework CoreServices
//              note: CoreServices needed for GetMacOSSStatusErrorString().
//
// Description:	This program reads in some MIDI data and the time stamps
//              which are attached to the data. Then it compares the
//              timestamp to the time value extracted from mach_absolute_time()
//
//              This program demonstrates the the MIDI time stamps are about
//              20 to 70 microseconds before the callback function receives
//              them.  This is well below the 300 microseconds per byte
//              data rate of standard MIDI.  This means that the timestamp
//              given with the data is the time at when the last byte
//              in the message was received, and not the time when the
//              first byte in the message arrived, or the time when the
//              first byte in the message started to arrive (which might
//              be a more preferable number).
//
//

#if defined(__APPLE__)

#include <CoreServices/CoreServices.h>
#include <CoreMIDI/CoreMIDI.h>  /* interface to MIDI in Macintosh OS X */
#include <stdio.h>
#include <mach/mach_time.h>    /* for Apple OS X timing functions */

void   printPacketInfo          (const MIDIPacket* packet);
void   myReadProc               (const MIDIPacketList *packetList, 
                                 void* readProcRefCon, void* srcConnRefCon);

/////////////////////////////////////////////////////////////////////////

int main(void) {
    
    // Prepare MIDI Interface Client/Port for writing MIDI data:
    MIDIClientRef midiclient  = NULL;
    MIDIPortRef   midiin     = NULL;
    OSStatus status;
    if ((status = MIDIClientCreate(CFSTR("TeStInG"), NULL, NULL, &midiclient)))
    {
        printf("Error trying to create MIDI Client structure: %d\n", (int)status);
        printf("%s\n", GetMacOSStatusErrorString(status));
        exit(status);
    }
    if ((status = MIDIInputPortCreate(midiclient, CFSTR("InPuT"), myReadProc, NULL, &midiin)))
    {
        printf("Error trying to create MIDI output port: %d\n", (int)status);
        printf("%s\n", GetMacOSStatusErrorString(status));
        exit(status);
    }
    
    ItemCount nSrcs = MIDIGetNumberOfSources();
    ItemCount iSrc;
    for (iSrc = 0; iSrc < nSrcs; iSrc++)
    {
        MIDIEndpointRef src = MIDIGetSource(iSrc);
        MIDIPortConnectSource(midiin, src, NULL);
    }
    
    CFRunLoopRef runLoop;
    runLoop = CFRunLoopGetCurrent();
    CFRunLoopRun();
    
    return 0;
}


/////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// myReadProc -- What to do when MIDI input packets are received.
//      used as an input parameter to MIDIInputPortCreate() so that 
//      MIDI input knows what to do with the MIDI messages after it
//      receives them.

void myReadProc(const MIDIPacketList *packetList, void* readProcRefCon, void* srcConnRefCon)
{
    MIDIPacket *packet = (MIDIPacket*)packetList->packet;
    int j;
    int count = packetList->numPackets;
    for (j = 0; j < count; j++)
    {
        printPacketInfo(packet);
        packet = MIDIPacketNext(packet);
    }
}



//////////////////////////////
//
// printPacketInfo --
//

void printPacketInfo(const MIDIPacket* packet)
{
    double timeinsec = 0.0;
    mach_timebase_info_data_t info;
    if (mach_timebase_info(&info) == 0)
    {
        timeinsec = mach_absolute_time()*1e-9*(double)info.numer/info.denom;
    }
    
    double stampinsec = packet->timeStamp / (double)1e9;
    printf("%9.3lfs\t-\t", timeinsec);
    printf("%9.3lfs\t", stampinsec);
    // display different in times in microseconds
    printf("%9.0lfus\t", (timeinsec-stampinsec)*1000000); 

    int midistatus = packet->data[0] & 0xf0;
    int midichannel = packet->data[0] & 0x0f;
        
    switch (midistatus)
    {
        // note off
        case 0x80:
            printf("note off (%d): note: %d velocity: %d", midichannel, packet->data[1], packet->data[2]);
            break;
                
        // note on
        case 0x90:
            printf("note on (%d): note: %d velocity: %d", midichannel, packet->data[1], packet->data[2]);
            break;
                
        // control change
        case 0xb0:
            printf("control change (%d): controller: %d value: %d", midichannel, packet->data[1], packet->data[2]);
            break;
    }
    printf("\n");
}

#else

int main()
{
    return 0;
}

#endif