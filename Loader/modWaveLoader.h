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
        /**
        this module loads wavefiles with different samplerates, resolutions and compression-types

        supported resolutions:
        - 8 bits
        - 16 bits
        - 24 bits
        - 32 bits integer/float
        - 64 bits integer/float

        */
        class WaveLoader : public Base::BaseModule
        {
        public:
            enum { MAX_WAVE_BUFFER = 0x10000, MAX_CHANNEL_NUM = 6 };

            /**
            enumeration of compression-types
            */
            enum
            {
                WAVE_COMPR_UNKNWON = 0,             ///< unknown compression
                WAVE_COMPR_PCM,                     ///< pcm-compression (integer)
                WAVE_COMPR_MS_ADPCM,                ///< Microsoft ADPCM
                WAVE_COMPR_IEEE,                    ///< Floating Point
                WAVE_COMPR_ALAW = 6,                ///< a-law compression
                WAVE_COMPR_ULAW,                    ///< mu-law compression
                WAVE_COMPR_IMA_ADPCM = 17,          ///< IMA-ADPCM
                WAVE_COMPR_EXPERIMENTAL = 0xFFFF    ///< experimental (should not appear)
            };

            /**
            enumeration of number of channels
            */
            enum
            {
                WAVE_FORMAT_MONO = 1,       ///< monophonic
                WAVE_FORMAT_STEREO = 2,     ///< stereophonic
                WAVE_FORMAT_SURROUND = 6    ///< surround-sound
            };

            /**
            enumeration of sample-sizes
            */
            enum
            {
                WAVE_SAMPLE_SIZE_8 = 8,     ///< 8 bits
                WAVE_SAMPLE_SIZE_16 = 16,   ///< 16 bits
                WAVE_SAMPLE_SIZE_24 = 24,   ///< 24 bits
                WAVE_SAMPLE_SIZE_32 = 32,   ///< 32 bits
                WAVE_SAMPLE_SIZE_64 = 64,   ///< 64 bits
            };

            /**
            enumeration for return codes
            */
            enum WaveLoaderError
            {
                WAVE_ERROR_NO_WAVE = -3,
                WAVE_ERROR_NO_RIFF,
                WAVE_ERROR_OPEN,
                WAVE_ERROR_NO_ERROR
            };

            /**
            header of wavefile
            */
            struct waveHeader
            {
                BYTE Magic[4];              ///< should be "RIFF"
                ULONG Size;                 ///< Filesize in Bytes without Header and Size
                BYTE RiffType[4];           ///< should be "WAVE"
            };

            /**
            chunk of data
            */
            struct chunkData
            {
                BYTE ChunkID[4];            ///< Type of WaveChunk
                ULONG ChunkSize;            ///< Size of Chunk
                BYTE* Data;                 ///< Data of Chunk
            };

            /**
            format chunk
            */
            typedef struct fmtChunk
            {
                USHORT Compression;         ///< Type of Compression
                USHORT NumChannels;         ///< Number of Channels (1 = Mono, 2 = Stereo, ...)
                ULONG SampleRate;           ///< Samplerate in Hertz
                ULONG BytesPerSec;          ///< Average Bytes per Second
                USHORT BlockAlign;          ///< Block Align (bytes per frame * channels)
                USHORT BitsPerSample;       ///< Significant Bits per Sample
                USHORT ExtraFormatLng;      ///< Length of Extra Format
                BYTE* ExtraFormatData;      ///< Extra Format Bytes
            } WaveFormat;

            /**
            data chunk
            */
            struct dataChunk
            {
                __int32** DataPtr;           ///< pointer to memory
            };

            /**
            structure of wavefile including header, format and data
            */
            struct waveFile
            {
                struct waveHeader Header;   ///< header of file
                struct fmtChunk format;     ///< format of file
                struct dataChunk data;      ///< data of file
            };

        public:
            /// default constuctor
            WaveLoader() : Base::BaseName("WaveLoader") { Init(); }
            ~WaveLoader();

        public:
            /**
            load a wavefile
            @param filename name of file
            @return error code
            */
            WaveLoaderError Load(std::string filename);

            /**
            unload previously loaded wavefile
            */
            void Unload(void);

        public:
            /**
            get pointer to data for specified channel
            @param channel the channel to retrieve data for
            @return pointer to data
            */
            __int32 *getWaveData(BYTE channel);

            /**
            get size of loaded wavefile in samples
            @return number of samples
            */
            long getWaveSize(void) { return SizeOfData; }

            /**
            get format of loaded wavefile
            @return format of file
            */
            const WaveFormat& getWaveFormat() { return Wave.format; }

        private:
            void Init(void);

        private:
            struct waveFile Wave;
            struct chunkData Chunk;
            __int32 *WaveData[MAX_CHANNEL_NUM];
            long SizeOfData;
        };
    }
}

#endif