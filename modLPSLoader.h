/*
 * modLPSLoader.h
 *
 *  Created on: 03.12.2011
 *      Author: dedokter
 */

#ifndef MODLPSLOADER_H_
#define MODLPSLOADER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <memory.h>
#include <zlib.h>
#include "modBaseModule.h"
#include "modTypes.h"

#define LPS_DEBUG_MODE  1

#define LPS_MAGIC       "sLps"
#define LPS_MAXTRACKS   16
#define LPS_MAXLENTRACK 10000000
#define LPS_MAXREADBUF  0x80000

namespace eLibV2
{
    enum
    {
        LPS_ERROR_NONE = 0,
        LPS_ERROR_FILENOTFOUND,
        LPS_ERROR_FILETYPE,
        LPS_ERROR_MEMORY_TRACK,
        LPS_ERROR_MEMORY_TRACKDATA
    };

    struct lpsData
    {
        BYTE    Name[64];                       // Name der Spur
        ULONG   LengthCompressed;               // L�nge der komprimierten Spur
        ULONG   LengthUnCompressed;             // L�nge der unkomprimierten Spur
        double  *Data;                          // Daten
    };

    struct lpsHeader
    {
        BYTE    Magic[4];                       // "sLps"-Kennung
        ULONG   Version;                        // Erstellt f�r Version
        BYTE    Name[64];                       // Loop-Name
        USHORT  BPM;                            // Geschwindigkeit
        ULONG   MaxLengthTracks;                // L�nge der l�ngsten Spur
        USHORT  NumTracks;                      // Anzahl Spuren
        lpsData *TrackData[LPS_MAXTRACKS];      // Daten der einzelnen Spuren
    };

    class LPSLoader : public BaseModule
    {
    public:
        LPSLoader();
        ~LPSLoader();

        void Init(void);
        void Reset(void);

        int Load(std::string filename);
        void Unload(void);

        // getter methods for LPSfile
        void getName(char *name);
        USHORT getBPM(void);
        ULONG getMaxLengthTracks(void);
        USHORT getNumTracks(void);

        // getter methods for LPStracks
        void getTrackName(char *name, VstInt16 TrackIndex);
        ULONG getTrackSize(VstInt16 TrackIndex);
        double *getTrackPointer(VstInt16 TrackIndex);
        double getTrackData(VstInt16 TrackIndex, VstInt16 DataIndex);

    private:
        BYTE *TrackCompressed, *TempBuffer;
        ULONG LengthUncompressed;

        // file and data pointers
        struct lpsHeader LPSFile;
        struct lpsData   *LPSTracks[LPS_MAXTRACKS];
        double *TrackData[LPS_MAXTRACKS];

        bool FileLoaded;

    #if LPS_DEBUG_MODE
        char debug_temp[1024];
    #endif
    };
}

#endif /* MODLPSLOADER_H_ */
