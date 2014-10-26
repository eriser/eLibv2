/*
 * modWaveLoader.cpp
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#include <Loader/modWaveLoader.h>

using namespace eLibV2;
using namespace std;

WaveLoader::WaveLoader()
{
    Init();
}

WaveLoader::~WaveLoader()
{
    if (WaveData)
        Unload();
}

void WaveLoader::Init()
{
    setModuleName("WaveLoader");
    WaveData = 0;
}

int WaveLoader::Load(std::string filename)
{
    ifstream wavefile;
    ULONG i, NumRemain, NumPages;
    BYTE buf[MAX_WAVE_BUFFER];
    BYTE *TempByteBuffer;
    SSHORT help = 0;

    wavefile.open(filename.c_str(), ifstream::in | ifstream::binary);
    if (!wavefile.good())
    {
#if WAVE_DEBUG_MODE
        sprintf(debug_temp, "error opening '%s'", filename.c_str());
        dbgOutput(debug_temp);
#endif
        return -1;
    }

    wavefile.read((char*)Wave.Header.Magic, sizeof(BYTE) * sizeof(Wave.Header.Magic));
    if (strncmp((char*)Wave.Header.Magic, WAVE_MAGIC_RIFF, 4))
    {
#if WAVE_DEBUG_MODE
        sprintf(debug_temp, "Error not a valid RIFF-File");
        dbgOutput(debug_temp);
#endif
        wavefile.close();
        return -2;
    }

    wavefile.read((char*)&Wave.Header.Size, sizeof(BYTE) * sizeof(Wave.Header.Size));
#if WAVE_DEBUG_MODE
    sprintf(debug_temp, "filesize: %li", Wave.Header.Size);
    dbgOutput(debug_temp);
#endif
    wavefile.read((char*)&Wave.Header.RiffType, sizeof(BYTE) * sizeof(Wave.Header.RiffType));
    if (strncmp((char*)Wave.Header.RiffType, WAVE_MAGIC_WAVE, 4))
    {
#if WAVE_DEBUG_MODE
        sprintf(debug_temp, "Error not a valid WAVE-File");
        dbgOutput(debug_temp);
#endif
        wavefile.close();
        return -3;
    }

    while (wavefile.good())
    {
        wavefile.read((char*)&Chunk.ChunkID, sizeof(BYTE) * sizeof(Chunk.ChunkID));
        wavefile.read((char*)&Chunk.ChunkSize, sizeof(BYTE) * sizeof(Chunk.ChunkSize));

        // format chunk
        if (!strncmp((char*)&Chunk.ChunkID, WAVE_MAGIC_FMT, 4))
        {
#if WAVE_DEBUG_MODE
            sprintf(debug_temp, "fmt-Chunk size: %li", Chunk.ChunkSize);
            dbgOutput(debug_temp);
#endif
            wavefile.read((char*)&Wave.fmt.Compression, sizeof(BYTE) * sizeof(Wave.fmt.Compression));
            wavefile.read((char*)&Wave.fmt.NumChannels, sizeof(BYTE) * sizeof(Wave.fmt.NumChannels));
            wavefile.read((char*)&Wave.fmt.SampleRate, sizeof(BYTE) * sizeof(Wave.fmt.SampleRate));
            wavefile.read((char*)&Wave.fmt.BytesPerSec, sizeof(BYTE) * sizeof(Wave.fmt.BytesPerSec));
            wavefile.read((char*)&Wave.fmt.BlockAlign, sizeof(BYTE) * sizeof(Wave.fmt.BlockAlign));
            wavefile.read((char*)&Wave.fmt.BitsPerSample, sizeof(BYTE) * sizeof(Wave.fmt.BitsPerSample));
#if WAVE_DEBUG_MODE
            sprintf(debug_temp, "Compression: %i", Wave.fmt.Compression);
            dbgOutput(debug_temp);
            sprintf(debug_temp, "Number of Channels: %i", Wave.fmt.NumChannels);
            dbgOutput(debug_temp);
            sprintf(debug_temp, "Samplerate: %li Hz", Wave.fmt.SampleRate);
            dbgOutput(debug_temp);
            sprintf(debug_temp, "bytes/sec: %li", Wave.fmt.BytesPerSec);
            dbgOutput(debug_temp);
            sprintf(debug_temp, "BlockAlign: %i", Wave.fmt.BlockAlign);
            dbgOutput(debug_temp);
            sprintf(debug_temp, "bits/sample: %i", Wave.fmt.BitsPerSample);
            dbgOutput(debug_temp);
#endif
            if (Chunk.ChunkSize > 16)
            {
                wavefile.read((char*)&Wave.fmt.ExtraFormatLng, sizeof(BYTE) * sizeof(Wave.fmt.ExtraFormatLng));
#if WAVE_DEBUG_MODE
                sprintf(debug_temp, "Extra Format Length: %i", Wave.fmt.ExtraFormatLng);
                dbgOutput(debug_temp);
#endif
                if (Wave.fmt.ExtraFormatLng)
                {
                    for (i = 0; i < Wave.fmt.ExtraFormatLng; i++)
                        wavefile.read((char*)buf, sizeof(BYTE));
                }
            }
        }
        // data chunk
        else if (!strncmp((char*)&Chunk.ChunkID, WAVE_MAGIC_DATA, 4))
        {
#if WAVE_DEBUG_MODE
            sprintf(debug_temp, "data-Chunk size: %li", Chunk.ChunkSize);
            dbgOutput(debug_temp);
#endif
            TempByteBuffer = (BYTE*)malloc(Chunk.ChunkSize);

            NumRemain = Chunk.ChunkSize % MAX_WAVE_BUFFER;
            NumPages = (Chunk.ChunkSize - NumRemain) / MAX_WAVE_BUFFER;
            for (i = 0; i < NumPages; i++)
            {
                wavefile.read((char*)buf, sizeof(BYTE) * MAX_WAVE_BUFFER);
                memcpy(&TempByteBuffer[i * MAX_WAVE_BUFFER], buf, MAX_WAVE_BUFFER);
            }
            if (NumRemain)
            {
                wavefile.read((char*)buf, sizeof(BYTE) * MAX_WAVE_BUFFER);
                memcpy(&TempByteBuffer[i * MAX_WAVE_BUFFER], buf, NumRemain);
            }
            SizeOfData = Chunk.ChunkSize / 2;
            WaveData = new double[SizeOfData];

            for (long numread = 0; numread < SizeOfData; numread++)
            {
                help = (TempByteBuffer[numread * 2 + 1] << 8) + TempByteBuffer[numread * 2];
                WaveData[numread] = ((double)(help)) / 0x10000;
            }
            free(TempByteBuffer);
            TempByteBuffer = 0;
            Wave.data.Data = WaveData;
        }
        else
        {
#if WAVE_DEBUG_MODE
            sprintf(debug_temp, "unknown chunk: '%c%c%c%c' size: %li", Chunk.ChunkID[0], Chunk.ChunkID[1], Chunk.ChunkID[2], Chunk.ChunkID[3], Chunk.ChunkSize);
            dbgOutput(debug_temp);
#endif
            for (i = 0; i < Chunk.ChunkSize; i++)
            {
                wavefile.read((char*)buf, 1);
            }
        }
    }
    wavefile.close();
    return 0;
}

void WaveLoader::Unload(void)
{
    if (WaveData)
        delete WaveData;
    WaveData = 0;
}

void WaveLoader::getWaveFormat(WaveFormat *Format)
{
    if (WaveData)
        memcpy((void*)Format, &Wave.fmt, sizeof(Wave.fmt));
}

void WaveLoader::runTests(void)
{
}