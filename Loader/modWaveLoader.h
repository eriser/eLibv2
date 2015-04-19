#ifndef MODWAVELOADER_H_
#define MODWAVELOADER_H_

#include <Base/modBaseModule.h>
#include <Util/modTypes.h>

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <memory.h>

// Wave Formats and Subchunks
#define WAVE_MAGIC_RIFF "RIFF"
#define WAVE_MAGIC_WAVE "WAVE"
#define WAVE_MAGIC_FMT  "fmt "
#define WAVE_MAGIC_DATA "data"
#define WAVE_MAGIC_FACT "fact"
#define WAVE_MAGIC_CUE  "cue "
#define WAVE_MAGIC_PLST "plst"
#define WAVE_MAGIC_LIST "list"
#define WAVE_MAGIC_LABL "labl"
#define WAVE_MAGIC_LTXT "ltxt"
#define WAVE_MAGIC_NOTE "note"
#define WAVE_MAGIC_SMPL "smpl"
#define WAVE_MAGIC_INST "inst"

namespace eLibV2
{
    namespace Loader
    {
        class WaveLoader : public Base::BaseModule
        {
        public:
            enum { MAX_WAVE_BUFFER = 0x10000, MAX_CHANNEL_NUM = 6 };

            enum
            {
                WAVE_COMPR_UNKNWON = 0,
                WAVE_COMPR_PCM,
                WAVE_COMPR_MS_ADPCM,
                WAVE_COMPR_IEEE,
                WAVE_COMPR_ALAW = 6,
                WAVE_COMPR_ULAW,
                WAVE_COMPR_IMA_ADPCM = 17,
                WAVE_COMPR_EXPERIMENTAL = 0xFFFF
            };

            enum
            {
                WAVE_FORMAT_MONO = 1,
                WAVE_FORMAT_STEREO = 2
            };

            enum
            {
                WAVE_SAMPLE_SIZE_8 = 8,
                WAVE_SAMPLE_SIZE_16 = 16,
                WAVE_SAMPLE_SIZE_24 = 24,
                WAVE_SAMPLE_SIZE_32 = 32,
                WAVE_SAMPLE_SIZE_64 = 64,
            };

            // Data Compression
            struct waveHeader
            {
                BYTE Magic[4];              // should be "RIFF"
                ULONG Size;                 // Filesize in Bytes without Header and Size
                BYTE RiffType[4];           // should be "WAVE"
            };

            struct chunkData
            {
                BYTE ChunkID[4];            // Type of WaveChunk
                ULONG ChunkSize;            // Size of Chunk
                BYTE* Data;                 // Data of Chunk
            };

            typedef struct fmtChunk
            {
                USHORT Compression;         // Type of Compression
                USHORT NumChannels;         // Number of Channels (1 = Mono, 2 = Stereo)
                ULONG SampleRate;           // Samplerate in Hertz
                ULONG BytesPerSec;          // Average Bytes per Second
                USHORT BlockAlign;          // Block Align
                USHORT BitsPerSample;       // Significant Bits per Sample
                USHORT ExtraFormatLng;      // Length of Extra Format
                BYTE* ExtraFormatData;      // Extra Format Bytes
            } WaveFormat;

            struct dataChunk
            {
                __int32** DataPtr;           // Data
            };

            struct waveFile
            {
                struct waveHeader Header;
                struct fmtChunk format;
                struct dataChunk data;
            };

        public:
            WaveLoader() : Base::BaseName("WaveLoader") { Init(); }
            ~WaveLoader();

        public:
            void Init(void);

        public:
            int Load(std::string filename);
            void Unload(void);

        public:
            __int32 *getWaveData(void) { return WaveData[0]; }
            __int32 *getWaveData(BYTE channel)
            {
                if (channel < Wave.format.NumChannels)
                    return WaveData[channel];
                else
                    return NULL;
            }
            long getWaveSize(void) { return SizeOfData; }
            const WaveFormat& getWaveFormat();

        private:
            struct waveFile Wave;
            struct chunkData Chunk;
            __int32 *WaveData[MAX_CHANNEL_NUM];
            long SizeOfData;
        };
    }
}

#endif