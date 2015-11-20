#ifndef MODLPSLOADER_H_
#define MODLPSLOADER_H_

#include <Base/BaseModule.h>

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
                UInt8   Name[64];                       // Name der Spur
                UInt32  LengthCompressed;               // L�nge der komprimierten Spur
                UInt32  LengthUnCompressed;             // L�nge der unkomprimierten Spur
                double  *Data;                          // Daten
            };

            struct lpsHeader
            {
                UInt8   Magic[4];                       // "sLps"-Kennung
                UInt32  Version;                        // Erstellt f�r Version
                UInt8   Name[64];                       // Loop-Name
                UInt16  BPM;                            // Geschwindigkeit
                UInt32  MaxLengthTracks;                // L�nge der l�ngsten Spur
                UInt16  NumTracks;                      // Anzahl Spuren
                lpsData *TrackData[LPS_MAXTRACKS];      // Daten der einzelnen Spuren
            };

        public:
            LPSLoader() :
                Base::BaseName("LPSLoader")
            {
                Init();
            }
            virtual ~LPSLoader();

            void Init(void);

            SInt16 Load(std::string filename);
            void Unload(void);

            // getter methods for LPSfile
            void getName(char *name);
            UInt16 getBPM(void);
            UInt32 getMaxLengthTracks(void);
            UInt16 getNumTracks(void);

            // getter methods for LPStracks
            void getTrackName(char *name, SInt16 TrackIndex);
            UInt32 getTrackSize(SInt16 TrackIndex);
            double *getTrackPointer(SInt16 TrackIndex);
            double getTrackData(SInt16 TrackIndex, SInt16 DataIndex);

        private:
            UInt8 *TrackCompressed, *TempBuffer;
            UInt32 LengthUncompressed;

            // file and data pointers
            struct lpsHeader LPSFile;
            struct lpsData   *LPSTracks[LPS_MAXTRACKS];
            double *TrackData[LPS_MAXTRACKS];

            bool FileLoaded;
        };
    }
}

#endif
