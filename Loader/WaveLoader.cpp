#include <Loader/WaveLoader.h>
#include <cstdlib>

using namespace eLibV2::Loader;
using namespace eLibV2::Util;

WaveLoader::~WaveLoader()
{
    if (WaveData)
        Unload();
}

void WaveLoader::Init()
{
    m_bLoaded = false;
    memset(&Wave, 0, sizeof(Wave));
    SizeOfData = 0;
    fScaler32 = (double)0x7fffffffL;
}

float *WaveLoader::getWaveData(UInt8 channel)
{
    if (m_bLoaded && channel < Wave.format.NumChannels)
        return WaveData[channel];
    else
        return NULL;
}

WaveLoader::WaveLoaderError WaveLoader::Load(std::string filename)
{
    std::ifstream wavefile;
    UInt32 i, NumRemain, NumPages;
    UInt8* buf = new UInt8[MAX_WAVE_BUFFER];
    UInt8 *TempByteBuffer;

    wavefile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!wavefile.good())
    {
        ModuleLogger::print(LOG_CLASS_LOADER, "error opening '%s'", filename.c_str());
        return WAVE_ERROR_OPEN;
    }

    wavefile.read((char*)Wave.Header.Magic, sizeof(Wave.Header.Magic));
    if (strncmp((char*)Wave.Header.Magic, WAVE_MAGIC_RIFF, 4))
    {
        ModuleLogger::print(LOG_CLASS_LOADER, "Error not a valid RIFF-File");
        wavefile.close();
        return WAVE_ERROR_NO_RIFF;
    }

    wavefile.read((char*)&Wave.Header.Size, sizeof(Wave.Header.Size));
    ModuleLogger::print(LOG_CLASS_LOADER, "filesize: %li", Wave.Header.Size);

    wavefile.read((char*)&Wave.Header.RiffType, sizeof(Wave.Header.RiffType));
    if (strncmp((char*)Wave.Header.RiffType, WAVE_MAGIC_WAVE, 4))
    {
        ModuleLogger::print(LOG_CLASS_LOADER, "Error not a valid WAVE-File");
        wavefile.close();
        return WAVE_ERROR_NO_WAVE;
    }

    while (wavefile.good())
    {
        wavefile.read((char*)&Chunk.ChunkID, sizeof(Chunk.ChunkID));
        wavefile.read((char*)&Chunk.ChunkSize, sizeof(Chunk.ChunkSize));

        if (Chunk.ChunkSize % 4 != 0)
        {
            ModuleLogger::print(LOG_CLASS_LOADER, "Chunk size not aligned. will be padded");
            Chunk.ChunkSize += (4 - (Chunk.ChunkSize % 4));
        }

        // format chunk
        if (strncmp((char*)&Chunk.ChunkID, WAVE_MAGIC_FMT, 4) == 0)
        {
            ModuleLogger::print(LOG_CLASS_LOADER, "fmt-Chunk size: %li", Chunk.ChunkSize);
            wavefile.read((char*)&Wave.format.Compression, sizeof(Wave.format.Compression));
            wavefile.read((char*)&Wave.format.NumChannels, sizeof(Wave.format.NumChannels));
            wavefile.read((char*)&Wave.format.SampleRate, sizeof(Wave.format.SampleRate));
            wavefile.read((char*)&Wave.format.BytesPerSec, sizeof(Wave.format.BytesPerSec));
            wavefile.read((char*)&Wave.format.BlockAlign, sizeof(Wave.format.BlockAlign));
            wavefile.read((char*)&Wave.format.BitsPerSample, sizeof(Wave.format.BitsPerSample));
            ModuleLogger::print(LOG_CLASS_LOADER, "Compression: %i", Wave.format.Compression);
            ModuleLogger::print(LOG_CLASS_LOADER, "Number of Channels: %i", Wave.format.NumChannels);
            ModuleLogger::print(LOG_CLASS_LOADER, "Samplerate: %li Hz", Wave.format.SampleRate);
            ModuleLogger::print(LOG_CLASS_LOADER, "bytes/sec: %li", Wave.format.BytesPerSec);
            ModuleLogger::print(LOG_CLASS_LOADER, "BlockAlign: %i", Wave.format.BlockAlign);
            ModuleLogger::print(LOG_CLASS_LOADER, "bits/sample: %i", Wave.format.BitsPerSample);
            if (Chunk.ChunkSize > 16)
            {
                wavefile.read((char*)&Wave.format.ExtraFormatLng, sizeof(Wave.format.ExtraFormatLng));
                ModuleLogger::print(LOG_CLASS_LOADER, "Extra Format Length: %i", Wave.format.ExtraFormatLng);
                if (Wave.format.ExtraFormatLng)
                {
                    for (i = 0; i < Wave.format.ExtraFormatLng; i++)
                        wavefile.read((char*)buf, sizeof(UInt8));
                }
            }
        }
        // data chunk
        else if (strncmp((char*)&Chunk.ChunkID, WAVE_MAGIC_DATA, 4) == 0)
        {
            ModuleLogger::print(LOG_CLASS_LOADER, "data-Chunk size: %li", Chunk.ChunkSize);
            TempByteBuffer = (UInt8*)malloc(Chunk.ChunkSize);

            NumRemain = Chunk.ChunkSize % MAX_WAVE_BUFFER;
            NumPages = (Chunk.ChunkSize - NumRemain) / MAX_WAVE_BUFFER;
            ModuleLogger::print(LOG_CLASS_LOADER, "reading %li pages with %li bytes and %li remaining bytes.", NumPages, MAX_WAVE_BUFFER, NumRemain);
            for (i = 0; i < NumPages; i++)
            {
                wavefile.read((char*)buf, MAX_WAVE_BUFFER);
                memcpy(&TempByteBuffer[i * MAX_WAVE_BUFFER], buf, MAX_WAVE_BUFFER);
            }
            if (NumRemain)
            {
                wavefile.read((char*)buf, NumRemain);
                memcpy(&TempByteBuffer[i * MAX_WAVE_BUFFER], buf, NumRemain);
            }
            SizeOfData = Chunk.ChunkSize / ((Wave.format.BitsPerSample / 8) * Wave.format.NumChannels);

            WaveData = new float*[Wave.format.NumChannels];
            for (int channelIndex = 0; channelIndex < Wave.format.NumChannels; channelIndex++)
            {
                if (channelIndex >= MAX_CHANNEL_NUM)
                {
                    // since all data has been already read just break the processing, if too many channels
                    ModuleLogger::print(LOG_CLASS_LOADER, "data contains more than %li channels. remaining data ignored.", MAX_CHANNEL_NUM);
                    break;
                }

                // allocate memory for wavedata (will use 32bit-floats for storage disregarding input format)
                ModuleLogger::print(LOG_CLASS_LOADER, "allocating %li frames for data of channel %li.", SizeOfData, channelIndex);
                WaveData[channelIndex] = new float[SizeOfData];
                int byteAdvance = Wave.format.BlockAlign / Wave.format.NumChannels;

                SInt32 byteOffset = channelIndex * byteAdvance;
                ModuleLogger::print(LOG_CLASS_LOADER, "advancing %li bytes", byteAdvance);

                // iterate over samples
                for (SInt32 sampleIndex = 0; sampleIndex < SizeOfData; sampleIndex++)
                {
                    SInt32 tempData = 0;
                    // input data will be stored left-aligned
                    switch (Wave.format.BitsPerSample)
                    {
                        case WAVE_SAMPLE_SIZE_8:
                            tempData = (TempByteBuffer[byteOffset] << 24) + 0x00ffffff;
                            WaveData[channelIndex][sampleIndex] = Int32toFloat32(tempData);;
                            break;

                        case WAVE_SAMPLE_SIZE_16:
                            tempData = (TempByteBuffer[byteOffset + 1] << 24) + (TempByteBuffer[byteOffset] << 16)
                                + (TempByteBuffer[byteOffset + 1] << 8) + TempByteBuffer[byteOffset];
                            WaveData[channelIndex][sampleIndex] = Int32toFloat32(tempData);;
                            break;

                        case WAVE_SAMPLE_SIZE_24:
                            tempData = (TempByteBuffer[byteOffset + 2] << 24) + (TempByteBuffer[byteOffset + 1] << 16)
                                + (TempByteBuffer[byteOffset] << 8);
                            WaveData[channelIndex][sampleIndex] = Int32toFloat32(tempData);;
                            break;

                        case WAVE_SAMPLE_SIZE_32:
                        {
                            tempData = (TempByteBuffer[byteOffset + 3] << 24) + (TempByteBuffer[byteOffset + 2] << 16)
                                + (TempByteBuffer[byteOffset + 1] << 8) + TempByteBuffer[byteOffset];

                            switch (Wave.format.Compression)
                            {
                                case WAVE_COMPR_PCM:
                                    WaveData[channelIndex][sampleIndex] = Int32toFloat32(tempData);;
                                    break;

                                case WAVE_COMPR_IEEE:
                                    float *pf = (float*)(&tempData);
                                    WaveData[channelIndex][sampleIndex] = *pf;
                                    break;
                            }
                        }
                        break;

                        case WAVE_SAMPLE_SIZE_64:
                            // will use only highest four bytes, dropping the rest (keeps about 99.5% accuracy)
                            tempData = (TempByteBuffer[byteOffset + 7] << 24) + (TempByteBuffer[byteOffset + 6] << 16)
                                + (TempByteBuffer[byteOffset + 5] << 8) + TempByteBuffer[byteOffset + 4];

                            // compression should be floating point
                            switch (Wave.format.Compression)
                            {
                                case WAVE_COMPR_PCM:
                                    ModuleLogger::print(LOG_CLASS_LOADER, "64 bit integer wave?");
                                    WaveData[channelIndex][sampleIndex] = Int32toFloat32(tempData);;
                                    break;

                                case WAVE_COMPR_IEEE:
                                    float *pf = (float*)(&tempData);
                                    WaveData[channelIndex][sampleIndex] = *pf;
                                    break;
                            }
                            break;
                    }
                    byteOffset += Wave.format.BlockAlign;
                }
            }
            if (TempByteBuffer)
                free(TempByteBuffer);
            TempByteBuffer = NULL;

            Wave.data.DataPtr = WaveData;
        }
        // cue chunk
        else if (strncmp((char*)&Chunk.ChunkID, WAVE_MAGIC_CUE, 4) == 0)
        {
            UInt32 numCues;
            cueChunk cuePoint;
            wavefile.read((char*)&numCues, sizeof(numCues));
            ModuleLogger::print(LOG_CLASS_LOADER, "cue chunk: '%li'", numCues);
            for (i = 0; i < numCues; ++i)
            {
                wavefile.read((char*)&cuePoint, sizeof(cuePoint));
                ModuleLogger::print(LOG_CLASS_LOADER, "cue point: blockstart: %li chunkstart: %li position: %li sampleoffset: %li", cuePoint.dwBlockStart, cuePoint.dwChunkStart, cuePoint.dwPosition, cuePoint.dwSampleOffset);
            }
        }
        else
        {
            ModuleLogger::print(LOG_CLASS_LOADER, "unknown chunk: '%c%c%c%c' size: %li", Chunk.ChunkID[0], Chunk.ChunkID[1], Chunk.ChunkID[2], Chunk.ChunkID[3], Chunk.ChunkSize);
            for (i = 0; i < Chunk.ChunkSize; i++)
            {
                wavefile.read((char*)buf, 1);
            }
        }
    }

    if (buf)
        delete[] buf;
    buf = NULL;

    wavefile.close();
    m_bLoaded = true;
    return WAVE_ERROR_NO_ERROR;
}

void WaveLoader::Unload(void)
{
    if (m_bLoaded && Wave.format.NumChannels > 0)
    {
        for (int channelIndex = 0; channelIndex < Wave.format.NumChannels; channelIndex++)
        {
            if (WaveData[channelIndex])
                delete[] WaveData[channelIndex];
            WaveData[channelIndex] = NULL;
        }
        delete[] WaveData;
        m_bLoaded = false;
    }
}
