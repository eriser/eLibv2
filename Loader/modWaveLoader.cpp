#include <Loader/modWaveLoader.h>

using namespace eLibV2;

WaveLoader::~WaveLoader()
{
    if (WaveData)
        Unload();
}

void WaveLoader::Init()
{
    WaveData = 0;
}

int WaveLoader::Load(std::string filename)
{
    std::ifstream wavefile;
    ULONG i, NumRemain, NumPages;
    BYTE buf[MAX_WAVE_BUFFER];
    BYTE *TempByteBuffer;
    SSHORT help = 0;

    wavefile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!wavefile.good())
    {
        ModuleLogger::print("error opening '%s'", filename.c_str());
        return -1;
    }

    wavefile.read((char*)Wave.Header.Magic, sizeof(BYTE) * sizeof(Wave.Header.Magic));
    if (strncmp((char*)Wave.Header.Magic, WAVE_MAGIC_RIFF, 4))
    {
        ModuleLogger::print("Error not a valid RIFF-File");
        wavefile.close();
        return -2;
    }

    wavefile.read((char*)&Wave.Header.Size, sizeof(BYTE) * sizeof(Wave.Header.Size));
    ModuleLogger::print("filesize: %li", Wave.Header.Size);

    wavefile.read((char*)&Wave.Header.RiffType, sizeof(BYTE) * sizeof(Wave.Header.RiffType));
    if (strncmp((char*)Wave.Header.RiffType, WAVE_MAGIC_WAVE, 4))
    {
        ModuleLogger::print("Error not a valid WAVE-File");
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
            ModuleLogger::print("fmt-Chunk size: %li", Chunk.ChunkSize);
            wavefile.read((char*)&Wave.fmt.Compression, sizeof(BYTE) * sizeof(Wave.fmt.Compression));
            wavefile.read((char*)&Wave.fmt.NumChannels, sizeof(BYTE) * sizeof(Wave.fmt.NumChannels));
            wavefile.read((char*)&Wave.fmt.SampleRate, sizeof(BYTE) * sizeof(Wave.fmt.SampleRate));
            wavefile.read((char*)&Wave.fmt.BytesPerSec, sizeof(BYTE) * sizeof(Wave.fmt.BytesPerSec));
            wavefile.read((char*)&Wave.fmt.BlockAlign, sizeof(BYTE) * sizeof(Wave.fmt.BlockAlign));
            wavefile.read((char*)&Wave.fmt.BitsPerSample, sizeof(BYTE) * sizeof(Wave.fmt.BitsPerSample));
            ModuleLogger::print("Compression: %i", Wave.fmt.Compression);
            ModuleLogger::print("Number of Channels: %i", Wave.fmt.NumChannels);
            ModuleLogger::print("Samplerate: %li Hz", Wave.fmt.SampleRate);
            ModuleLogger::print("bytes/sec: %li", Wave.fmt.BytesPerSec);
            ModuleLogger::print("BlockAlign: %i", Wave.fmt.BlockAlign);
            ModuleLogger::print("bits/sample: %i", Wave.fmt.BitsPerSample);
            if (Chunk.ChunkSize > 16)
            {
                wavefile.read((char*)&Wave.fmt.ExtraFormatLng, sizeof(BYTE) * sizeof(Wave.fmt.ExtraFormatLng));
                ModuleLogger::print("Extra Format Length: %i", Wave.fmt.ExtraFormatLng);
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
            ModuleLogger::print("data-Chunk size: %li", Chunk.ChunkSize);
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
            ModuleLogger::print("unknown chunk: '%c%c%c%c' size: %li", Chunk.ChunkID[0], Chunk.ChunkID[1], Chunk.ChunkID[2], Chunk.ChunkID[3], Chunk.ChunkSize);
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

bool WaveLoader::Test(void)
{
    return true;
}