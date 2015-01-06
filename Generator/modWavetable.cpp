#include <Generator/modWavetable.h>

BaseWavetable::~BaseWavetable()
{
    for (VstInt16 WaveformIndex = 0; WaveformIndex < WAVEFORM_INDEX_MAX; WaveformIndex++)
        DeleteWaveform(WaveformIndex);
}

void BaseWavetable::Init()
{
    double random = 0.0, sh_random = 0.0;
    const static int q = 15;
    const static double c1 = (1 << q) - 1;
    const static double c2 = (double)(((int)(c1 / 3)) + 1);
    const static double c3 = 1.0 / c1;

    // builtin waveforms
    double *sine;
    double *enis;
    double *triangle;
    double *sawtooth1;
    double *sawtooth2;
    double *pulse;
    double *noise;
    double *smplhold;

    LoadedWaveforms = 0;
    srand((unsigned)time(NULL));

    try
    {
        // setup memory
        sine = new double[kWavesize];
        enis = new double[kWavesize];
        triangle = new double[kWavesize];
        sawtooth1 = new double[kWavesize];
        sawtooth2 = new double[kWavesize];
        pulse = new double[kWavesize];
        noise = new double[kWavesize];
        smplhold = new double[kWavesize];

        // create waveforms
        for (VstInt32 SampleIndex = 0; SampleIndex < kWavesize; SampleIndex++)
        {
            sine[SampleIndex] = sin((2.0 * PI * (SampleIndex / (double)kWavesize)));
            enis[SampleIndex] = -sine[SampleIndex];

            if (SampleIndex <= kWavesize / 4)
                triangle[SampleIndex] = 1.0 / (kWavesize / 4) * SampleIndex;
            if ((SampleIndex <= 3 * kWavesize / 4) && (SampleIndex > kWavesize / 4))
                triangle[SampleIndex] = -2.0 / (kWavesize / 2) * (SampleIndex - kWavesize / 4) + 1.0;
            if (SampleIndex > 3 * kWavesize / 4)
                triangle[SampleIndex] = 1.0 / (kWavesize / 4) * (SampleIndex - 3 * kWavesize / 4) - 1.0;

            if (SampleIndex < kWavesize / 2)
                sawtooth1[SampleIndex] = 1.0 / (kWavesize / 2) * SampleIndex;
            else
                sawtooth1[SampleIndex] = 1.0 / (kWavesize / 2) * (SampleIndex - kWavesize / 2) - 1.0;

            if (SampleIndex < kWavesize / 2)
                sawtooth2[SampleIndex] = -1.0 / (kWavesize / 2) * SampleIndex;
            else
                sawtooth2[SampleIndex] = -1.0 / (kWavesize / 2) * (SampleIndex - kWavesize / 2) + 1.0;

            if (SampleIndex == kWavesize / 2)
            {
                sawtooth1[SampleIndex] = 0.0;
                sawtooth2[SampleIndex] = 0.0;
            }

            if ((SampleIndex == 0) || (SampleIndex == kWavesize / 2))
                pulse[SampleIndex] = 0.0f;
            else
                pulse[SampleIndex] = (SampleIndex < (kWavesize / 2)) ? 1.0 : -1.0;

            random = ((float)rand() / (float)(RAND_MAX + 1));
            if (SampleIndex == 0)
                noise[SampleIndex] = 0.0f;
            else
            {
                if ((SampleIndex % 1024) == 0)
                    sh_random = random;

                noise[SampleIndex] = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
                smplhold[SampleIndex] =  (2.f * ((sh_random * c2) + (sh_random * c2) + (sh_random * c2)) - 3.f * (c2 - 1.f)) * c3;
            }
        }
        AddWaveform(sine, kWavesize, "sine", 1);
        AddWaveform(enis, kWavesize, "enis", 1);
        AddWaveform(triangle, kWavesize, "tri", 1);
        AddWaveform(sawtooth1, kWavesize, "sawup", 1);
        AddWaveform(sawtooth2, kWavesize, "sawdn", 1);
        AddWaveform(pulse, kWavesize, "pulse", 1);
        AddWaveform(noise, kWavesize, "noise", 1);
        AddWaveform(smplhold, kWavesize, "smhld", 1);

        if (sine)
            delete sine;
        if (enis)
            delete enis;
        if (triangle)
            delete triangle;
        if (sawtooth1)
            delete sawtooth1;
        if (sawtooth2)
            delete sawtooth2;
        if (pulse)
            delete pulse;
        if (noise)
            delete noise;
        if (smplhold)
            delete smplhold;
    }

    catch (...)
    {
        // reaching here lacks WAVEFORM_BUILTIN_NUM * kWavesize bytes of memory
		ModuleLogger::print("out of memory");
    }
}

void BaseWavetable::DeleteWaveform(VstInt32 Index)
{
    if (Waveforms[Index].WaveData)
    {
        delete Waveforms[Index].WaveData;
        Waveforms[Index].WaveData = 0;
    }
}

bool BaseWavetable::AddWaveform(std::string Filename, std::string WaveName)
{
    WaveLoader WL;
    WaveLoader::WaveFormat WF;
    double *WD;

    try
    {
        if (LoadedWaveforms >= WAVEFORM_INDEX_MAX)
            return false;

        if (WL.Load(Filename))
            return false;

        WL.getWaveFormat(&WF);
        Waveforms[LoadedWaveforms].ChannelNum = WF.NumChannels;
        Waveforms[LoadedWaveforms].WaveSize = WL.getWaveSize();
        Waveforms[LoadedWaveforms].WaveName.assign(WaveName);
        Waveforms[LoadedWaveforms].WaveData = new double[Waveforms[LoadedWaveforms].WaveSize];

        // copy wave data
        WD = WL.getWaveData();
        if (!WD)
            return false;

        for (VstInt32 SampleIndex = 0; SampleIndex < Waveforms[LoadedWaveforms].WaveSize / Waveforms[LoadedWaveforms].ChannelNum; SampleIndex += Waveforms[LoadedWaveforms].ChannelNum)
            Waveforms[LoadedWaveforms].WaveData[SampleIndex] = WD[SampleIndex];

        WL.Unload();
        LoadedWaveforms++;
    }

    catch (...)
    {
		ModuleLogger::print("out of memory");
        return false;
    }
    return true;
}

bool BaseWavetable::AddWaveform(double *Wavedata, VstInt32 WaveSize, std::string WaveName, VstInt16 ChannelNum)
{
    try
    {
        if (LoadedWaveforms >= WAVEFORM_INDEX_MAX)
            return false;

        Waveforms[LoadedWaveforms].ChannelNum = ChannelNum;
        Waveforms[LoadedWaveforms].WaveSize = WaveSize;
        Waveforms[LoadedWaveforms].WaveName.assign(WaveName);
        Waveforms[LoadedWaveforms].WaveData = new double[Waveforms[LoadedWaveforms].WaveSize];

        // copy wave data
        if (!Wavedata)
            return false;

        for (VstInt32 SampleIndex = 0; SampleIndex < Waveforms[LoadedWaveforms].WaveSize / Waveforms[LoadedWaveforms].ChannelNum; SampleIndex += Waveforms[LoadedWaveforms].ChannelNum)
            Waveforms[LoadedWaveforms].WaveData[SampleIndex] = Wavedata[SampleIndex];

        LoadedWaveforms++;
    }

    catch (...)
    {
		ModuleLogger::print("out of memory");
        return false;
    }
    return true;
}

#if defined(WIN32)
bool BaseWavetable::AddWaveform(HINSTANCE hInstance, VstInt32 ResourceID, std::string WaveName, VstInt16 ByteSize, VstInt16 ChannelNum)
{
    HGLOBAL hRData = NULL;
    HRSRC hResource = NULL;
    double *data;
    VstInt16 *pRData;
    VstInt16 *buf;
    VstInt32 bufferSize, Divisor;

    if (LoadedWaveforms >= WAVEFORM_INDEX_MAX)
        return false;

    try
    {
        // Load Resource
        hResource = FindResource(hInstance, MAKEINTRESOURCE(ResourceID), "RAW");
        bufferSize = SizeofResource(hInstance, hResource) / ByteSize;
        Waveforms[LoadedWaveforms].ChannelNum = ChannelNum;
        Waveforms[LoadedWaveforms].WaveSize = bufferSize;
        Waveforms[LoadedWaveforms].WaveName.assign(WaveName);
        Waveforms[LoadedWaveforms].WaveData = new double[Waveforms[LoadedWaveforms].WaveSize];

        data = Waveforms[LoadedWaveforms].WaveData;
        hRData = LoadResource(hInstance, hResource);

        if ((!data) || (!hRData))
            return false;

        pRData = (VstInt16 *)LockResource(hRData);
        UnlockResource(hRData);
        FreeResource(hRData);

        buf = pRData;
        Divisor = 1 << (ByteSize * 8);

        for (VstInt32 SampleIndex = 0; SampleIndex < Waveforms[LoadedWaveforms].WaveSize / Waveforms[LoadedWaveforms].ChannelNum; SampleIndex += Waveforms[LoadedWaveforms].ChannelNum)
            data[SampleIndex] = ((double)(buf[SampleIndex])) / Divisor;

        LoadedWaveforms++;
    }

    catch (...)
    {
		ModuleLogger::print("out of memory");
        return false;
    }
    return true;
}
#endif

double BaseWavetable::getWaveData(VstInt32 WaveIndex, double dPhase)
{
    double data = 0.0;

    if (WaveIndex >= LoadedWaveforms)
        return data;

    switch (WaveIndex)
    {
        case WAVEFORM_INDEX_NONE:
            data = 0.0;
            break;

        default:
            data = Waveforms[WaveIndex].WaveData[(VstInt32)(dPhase * 2) % Waveforms[WaveIndex].WaveSize];
            break;
    }
    data = minval(data, -1.0);
    data = maxval(data, 1.0);
    return data;
}

bool BaseWavetable::getWaveName(VstInt32 WaveIndex, char* name)
{
    if (WaveIndex >= LoadedWaveforms)
        return false;

    if (!name)
        return false;

    strcpy(name, Waveforms[WaveIndex].WaveName.c_str());
    return true;
}

long BaseWavetable::getWaveSize(VstInt16 WaveIndex)
{
    if (WaveIndex >= LoadedWaveforms)
        return 0;

    return Waveforms[WaveIndex].WaveSize;
}

double BaseWavetable::adjustPhase(VstInt16 WaveIndex, double phase)
{
    if (WaveIndex >= LoadedWaveforms)
        return 0.0;

    double dCorrect = (double)(getWaveSize(WaveIndex));
    if (phase >= dCorrect)
        return phase - dCorrect;
    else
        return phase;
}

bool BaseWavetable::Test(void)
{
	return true;
}