#ifndef MODWAVELOADER_H_
#define MODWAVELOADER_H_

#include <Base/modBaseModule.h>
#include <Util/modTypes.h>

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <memory.h>

#define WAVE_DEBUG_MODE  1

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

using namespace eLibV2;

namespace eLibV2
{
    enum { MAX_WAVE_BUFFER = 0x10000 };

    enum
    {
        WAVE_COMPR_UNKNWON = 0,
        WAVE_COMPR_PCM,
        WAVE_COMPR_MS_ADPCM,
        WAVE_COMPR_ALAW = 6,
        WAVE_COMPR_ULAW,
        WAVE_COMPR_IMA_ADPCM = 17,
        WAVE_COMPR_EXPERIMENTAL = 0xFFFF
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
        double* Data;               // Data
    };

    struct waveFile
    {
        struct waveHeader Header;
        WaveFormat fmt;
        struct dataChunk data;
    };

    class WaveLoader : public BaseModule
    {
    public:
		WaveLoader() : BaseName("WaveLoader") { Init(); }
        ~WaveLoader();

        void Init(void);

        int Load(std::string filename);
        void Unload(void);
        double *getWaveData(void) {return WaveData;}
        long getWaveSize(void) {return SizeOfData;}
        void getWaveFormat(WaveFormat *Format);
		void runTests(void);

    private:
        struct waveFile Wave;
        struct chunkData Chunk;
        double *WaveData;
        long SizeOfData;

    #if WAVE_DEBUG_MODE
        char debug_temp[1024];
    #endif
    };
}

#endif