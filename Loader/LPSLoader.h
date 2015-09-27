#ifndef MODLPSLOADER_H_
#define MODLPSLOADER_H_

#include <Base/BaseModule.h>
#include <Util/Types.h>

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <memory.h>
#include <zlib.h>

#define LPS_MAGIC       "sLps"

namespace eLibV2
{
    namespace Loader
    {
        class LPSLoader : public Base::BaseName
        {
        public:
            enum
            {
                LPS_MAXTRACKS = 16,
                LPS_MAXLENTRACK = 10 * 1024 * 1024,
                LPS_MAXREADBUF = 0x100000,
            };

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

        public:
            LPSLoader() : Base::BaseName("LPSLoader") { Init(); }
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
        };
    }
}

#endif
