#include <Loader/modWaveLoader.h>

using namespace eLibV2::Loader;
using namespace eLibV2::Util;

WaveLoader::~WaveLoader()
{
    if (WaveData)
        Unload();
}

void WaveLoader::Init()
{
    for (int channelIndex = 0; channelIndex < MAX_CHANNEL_NUM; channelIndex++)
        WaveData[channelIndex] = NULL;
    memset(&Wave, 0, sizeof(Wave));
    SizeOfData = 0;
}

__int32 *WaveLoader::getWaveData(BYTE channel)
{
    if (channel < Wave.format.NumChannels)
        return WaveData[channel];
    else
        return NULL;
}

WaveLoader::WaveLoaderError WaveLoader::Load(std::string filename)
{
    std::ifstream wavefile;
    ULONG i, NumRemain, NumPages;
    BYTE buf[MAX_WAVE_BUFFER];
    BYTE *TempByteBuffer;

    wavefile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!wavefile.good())
    {
        ModuleLogger::print("error opening '%s'", filename.c_str());
        return WAVE_ERROR_OPEN;
    }

    wavefile.read((char*)Wave.Header.Magic, sizeof(BYTE) * sizeof(Wave.Header.Magic));
    if (strncmp((char*)Wave.Header.Magic, WAVE_MAGIC_RIFF, 4))
    {
        ModuleLogger::print("Error not a valid RIFF-File");
        wavefile.close();
        return WAVE_ERROR_NO_RIFF;
    }

    wavefile.read((char*)&Wave.Header.Size, sizeof(BYTE) * sizeof(Wave.Header.Size));
    ModuleLogger::print("filesize: %li", Wave.Header.Size);

    wavefile.read((char*)&Wave.Header.RiffType, sizeof(BYTE) * sizeof(Wave.Header.RiffType));
    if (strncmp((char*)Wave.Header.RiffType, WAVE_MAGIC_WAVE, 4))
    {
        ModuleLogger::print("Error not a valid WAVE-File");
        wavefile.close();
        return WAVE_ERROR_NO_WAVE;
    }

    while (wavefile.good())
    {
        wavefile.read((char*)&Chunk.ChunkID, sizeof(BYTE) * sizeof(Chunk.ChunkID));
        wavefile.read((char*)&Chunk.ChunkSize, sizeof(BYTE) * sizeof(Chunk.ChunkSize));

        // format chunk
        if (strncmp((char*)&Chunk.ChunkID, WAVE_MAGIC_FMT, 4) == 0)
        {
            ModuleLogger::print("fmt-Chunk size: %li", Chunk.ChunkSize);
            wavefile.read((char*)&Wave.format.Compression, sizeof(BYTE) * sizeof(Wave.format.Compression));
            wavefile.read((char*)&Wave.format.NumChannels, sizeof(BYTE) * sizeof(Wave.format.NumChannels));
            wavefile.read((char*)&Wave.format.SampleRate, sizeof(BYTE) * sizeof(Wave.format.SampleRate));
            wavefile.read((char*)&Wave.format.BytesPerSec, sizeof(BYTE) * sizeof(Wave.format.BytesPerSec));
            wavefile.read((char*)&Wave.format.BlockAlign, sizeof(BYTE) * sizeof(Wave.format.BlockAlign));
            wavefile.read((char*)&Wave.format.BitsPerSample, sizeof(BYTE) * sizeof(Wave.format.BitsPerSample));
            ModuleLogger::print("Compression: %i", Wave.format.Compression);
            ModuleLogger::print("Number of Channels: %i", Wave.format.NumChannels);
            ModuleLogger::print("Samplerate: %li Hz", Wave.format.SampleRate);
            ModuleLogger::print("bytes/sec: %li", Wave.format.BytesPerSec);
            ModuleLogger::print("BlockAlign: %i", Wave.format.BlockAlign);
            ModuleLogger::print("bits/sample: %i", Wave.format.BitsPerSample);
            if (Chunk.ChunkSize > 16)
            {
                wavefile.read((char*)&Wave.format.ExtraFormatLng, sizeof(BYTE) * sizeof(Wave.format.ExtraFormatLng));
                ModuleLogger::print("Extra Format Length: %i", Wave.format.ExtraFormatLng);
                if (Wave.format.ExtraFormatLng)
                {
                    for (i = 0; i < Wave.format.ExtraFormatLng; i++)
                        wavefile.read((char*)buf, sizeof(BYTE));
                }
            }
        }
        // data chunk
        else if (strncmp((char*)&Chunk.ChunkID, WAVE_MAGIC_DATA, 4) == 0)
        {
            ModuleLogger::print("data-Chunk size: %li", Chunk.ChunkSize);
            TempByteBuffer = (BYTE*)malloc(Chunk.ChunkSize);

            NumRemain = Chunk.ChunkSize % MAX_WAVE_BUFFER;
            NumPages = (Chunk.ChunkSize - NumRemain) / MAX_WAVE_BUFFER;
            ModuleLogger::print("reading %li pages with %li bytes and %li remaining bytes.", NumPages, MAX_WAVE_BUFFER, NumRemain);
            for (i = 0; i < NumPages; i++)
            {
                wavefile.read((char*)buf, sizeof(BYTE) * MAX_WAVE_BUFFER);
                memcpy(&TempByteBuffer[i * MAX_WAVE_BUFFER], buf, MAX_WAVE_BUFFER);
            }
            if (NumRemain)
            {
                wavefile.read((char*)buf, sizeof(BYTE) * NumRemain);
                memcpy(&TempByteBuffer[i * MAX_WAVE_BUFFER], buf, NumRemain);
            }
            SizeOfData = Chunk.ChunkSize / Wave.format.BlockAlign;

            for (int channelIndex = 0; channelIndex < Wave.format.NumChannels; channelIndex++)
            {
                if (channelIndex >= MAX_CHANNEL_NUM)
                {
                    // since all data has been already read just break the processing, if too many channels
                    ModuleLogger::print("data contains more than %li channels. remaining data ignored.", MAX_CHANNEL_NUM);
                    break;
                }

                // allocate memory for wavedata (will use 32 bits for storage disregarding input format)
                ModuleLogger::print("allocating %li frames for data of channel %li.", SizeOfData, channelIndex);
                WaveData[channelIndex] = new __int32[SizeOfData];
                int byteAdvance = Wave.format.BlockAlign / Wave.format.NumChannels;

                long dataIndex = 0, numread;
                // iterate over samples
                for (numread = 0; numread < SizeOfData; numread += byteAdvance)
                {
                    // input data will be stored left-aligned
                    switch (Wave.format.BitsPerSample)
                    {
                    case WAVE_SAMPLE_SIZE_8:
                        WaveData[channelIndex][dataIndex++] = TempByteBuffer[numread] << 24;
                        break;

                    case WAVE_SAMPLE_SIZE_16:
                        WaveData[channelIndex][dataIndex++] = (TempByteBuffer[numread * byteAdvance + 1] << 24) + (TempByteBuffer[numread * byteAdvance] << 16);
                        break;

                    case WAVE_SAMPLE_SIZE_24:
                        WaveData[channelIndex][dataIndex++] = (TempByteBuffer[numread * byteAdvance + 2] << 24) + (TempByteBuffer[numread * byteAdvance + 1] << 16)
                            + (TempByteBuffer[numread * byteAdvance] << 8);
                        break;

                    case WAVE_SAMPLE_SIZE_32:
                    {
                        __int32 help = (TempByteBuffer[numread * byteAdvance + 3] << 24) + (TempByteBuffer[numread * byteAdvance + 2] << 16)
                            + (TempByteBuffer[numread * byteAdvance + 1] << 8) + TempByteBuffer[numread * byteAdvance];

                        switch (Wave.format.Compression)
                        {
                        case WAVE_COMPR_PCM:
                            WaveData[channelIndex][dataIndex++] = help;
                            break;

                        case WAVE_COMPR_IEEE:
                            __int32 *pi = &help;
                            double *pd = (double*)pi;
                            WaveData[channelIndex][dataIndex++] = *pi;
                            break;
                        }
                    }
                    break;

                    case WAVE_SAMPLE_SIZE_64:
                        // will use only highest four bytes, dropping the rest
                        __int32 help = (TempByteBuffer[numread * byteAdvance + 7] << 24) + (TempByteBuffer[numread * byteAdvance + 6] << 16)
                            + (TempByteBuffer[numread * byteAdvance + 5] << 8) + TempByteBuffer[numread * byteAdvance + 4];

                        // compression should be floating point
                        switch (Wave.format.Compression)
                        {
                        case WAVE_COMPR_PCM:
                            ModuleLogger::print("64 bit integer wave?");
                            break;

                        case WAVE_COMPR_IEEE:
                            __int32 *pi = &help;
                            double *pd = (double*)pi;
                            WaveData[channelIndex][dataIndex++] = *pi;
                            break;
                        }
                        break;
                    }
                }
                ModuleLogger::print("read %li frames", numread);
            }
            free(TempByteBuffer);
            TempByteBuffer = 0;
            Wave.data.DataPtr = WaveData;
        }
        else
        {
            ModuleLogger::print("unknown chunk: '%c%c%c%c' size: %li", Chunk.ChunkID[0], Chunk.ChunkID[1], Chunk.ChunkID[2], Chunk.ChunkID[3], Chunk.ChunkSize);
            for (i = 0; i < Chunk.ChunkSize; i++)
            {
                wavefile.read((char*)buf, 1);
            }
        }
    }
    wavefile.close();
    return WAVE_ERROR_NO_ERROR;
}

void WaveLoader::Unload(void)
{
    for (int channelIndex = 0; channelIndex < MAX_CHANNEL_NUM; channelIndex++)
    {
        if (WaveData[channelIndex])
            delete WaveData[channelIndex];
        WaveData[channelIndex] = NULL;
    }
}
