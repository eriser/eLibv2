#ifndef MODWAVELOADER_H_
#define MODWAVELOADER_H_

#error wwwdwdf
#include <Base/BaseModule.h>
#include <Util/Types.h>

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

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
        class WaveLoader : public Base::BaseName
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
                UInt8 Magic[4];              ///< should be "RIFF"
                UInt32 Size;                 ///< Filesize in Bytes without Header and Size
                UInt8 RiffType[4];           ///< should be "WAVE"
            };

            /**
            chunk of data
            */
            struct chunkData
            {
                UInt8 ChunkID[4];            ///< Type of WaveChunk
                UInt32 ChunkSize;            ///< Size of Chunk
                UInt8* Data;                 ///< Data of Chunk
            };

            /**
            format chunk
            */
            typedef struct fmtChunk
            {
                UInt16 Compression;         ///< Type of Compression
                UInt16 NumChannels;         ///< Number of Channels (1 = Mono, 2 = Stereo, ...)
                UInt32 SampleRate;           ///< Samplerate in Hertz
                UInt32 BytesPerSec;          ///< Average Bytes per Second
                UInt16 BlockAlign;          ///< Block Align (bytes per frame * channels)
                UInt16 BitsPerSample;       ///< Significant Bits per Sample
                UInt16 ExtraFormatLng;      ///< Length of Extra Format
                UInt8* ExtraFormatData;      ///< Extra Format Bytes
            } WaveFormat;

            /**
            data chunk
            */
            struct dataChunk
            {
                float** DataPtr;           ///< pointer to memory
            };

            /**
            cue chunk
            */
            struct cueChunk
            {
                UInt32  dwName;
                UInt32  dwPosition;
                UInt32  fccChunk;
                UInt32  dwChunkStart;
                UInt32  dwBlockStart;
                UInt32  dwSampleOffset;
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
            WaveLoader() :
                Base::BaseName("WaveLoader")
            {
                Init();
            }
            virtual ~WaveLoader();

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
            float *getWaveData(UInt8 channel);

            /**
            get size of loaded wavefile in samples
            @return number of samples
            */
            SInt32 getWaveSize(void) { return SizeOfData; }

            /**
            get format of loaded wavefile
            @return format of file
            */
            const WaveFormat& getWaveFormat() { return Wave.format; }

        private:
            void Init(void);
            float Int32toFloat32(SInt32 input)
            {
                double sc = fScaler32 + .49999;
                float output = ((double)input / sc);

                return output;
            }

        private:
            double fScaler32;
            struct waveFile Wave;
            struct chunkData Chunk;
            float **WaveData;
            SInt32 SizeOfData;
            bool m_bLoaded;
        };
    }
}

#endif