#include <Generator/modWavetable.h>

using namespace eLibV2::Generator;
using namespace eLibV2::Loader;
using namespace eLibV2::Util;

BaseWavetable* BaseWavetable::instance = 0;

BaseWavetable::~BaseWavetable()
{
    for (VstInt16 WaveformIndex = 0; WaveformIndex < Waveforms.size(); WaveformIndex++)
        DeleteWaveform(WaveformIndex);
}

BaseWavetable* BaseWavetable::getInstance()
{
    if (!instance)
        instance = new BaseWavetable();
    return instance;
}

void BaseWavetable::Init()
{
    double random = 0.0, sh_random = 0.0;
    const static int q = 15;
    const static double c1 = (1 << q) - 1;
    const static double c2 = (double)(((int)(c1 / 3)) + 1);
    const static double c3 = 1.0 / c1;

    // add empty waveform
    Waveform waveform = { 0 };
    Waveforms.push_back(waveform);

    // builtin waveforms
    double *sine;
    double *enis;
    double *triangle;
    double *sawtooth1;
    double *sawtooth2;
    double *pulse;
    double *noise;
    double *smplhold;

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
        if (sine)
            delete sine;

        AddWaveform(enis, kWavesize, "enis", 1);
        if (enis)
            delete enis;

        AddWaveform(triangle, kWavesize, "tri", 1);
        if (triangle)
            delete triangle;

        AddWaveform(sawtooth1, kWavesize, "sawup", 1);
        if (sawtooth1)
            delete sawtooth1;

        AddWaveform(sawtooth2, kWavesize, "sawdn", 1);
        if (sawtooth2)
            delete sawtooth2;

        AddWaveform(pulse, kWavesize, "pulse", 1);
        if (pulse)
            delete pulse;

        AddWaveform(noise, kWavesize, "noise", 1);
        if (noise)
            delete noise;

        AddWaveform(smplhold, kWavesize, "smhld", 1);
        if (smplhold)
            delete smplhold;
    }
    catch (std::bad_alloc& alloc)
    {
        // reaching here lacks WAVEFORM_BUILTIN_NUM * kWavesize bytes of memory
        ModuleLogger::print("Init: out of memory");
    }
    catch (...)
    {
        ModuleLogger::print("unknown exception");
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
    Waveform waveform;
    __int32 *WD;

    try
    {
        if (WL.Load(Filename))
            return false;

        WF = WL.getWaveFormat();
        waveform.ChannelNum = WF.NumChannels;
        waveform.WaveSize = WL.getWaveSize();
        waveform.WaveName.assign(WaveName);
        waveform.WaveData = new double[waveform.WaveSize];

        // copy wave data
        WD = WL.getWaveData();
        if (!WD)
            return false;

        for (VstInt32 SampleIndex = 0; SampleIndex < waveform.WaveSize / waveform.ChannelNum; SampleIndex += waveform.ChannelNum)
            waveform.WaveData[SampleIndex] = ((double)WD[SampleIndex]) / 0x10000000;

        WL.Unload();
        Waveforms.push_back(waveform);
    }
    catch (std::bad_alloc& alloc)
    {
        ModuleLogger::print("AddWaveform: out of memory (%s)", WaveName.c_str());
        return false;
    }
    catch (...)
    {
        ModuleLogger::print("unknown exception");
        return false;
    }
    return true;
}

bool BaseWavetable::AddWaveform(double *Wavedata, VstInt32 WaveSize, std::string WaveName, VstInt16 ChannelNum)
{
    Waveform waveform;

    try
    {
        ModuleLogger::print("AddWaveform: %s/%li", WaveName.c_str(), WaveSize);

        waveform.ChannelNum = ChannelNum;
        waveform.WaveSize = WaveSize;
        waveform.WaveName.assign(WaveName);
        waveform.WaveData = new double[waveform.WaveSize];

        // copy wave data
        if (!Wavedata)
            return false;

        for (VstInt32 SampleIndex = 0; SampleIndex < waveform.WaveSize / waveform.ChannelNum; SampleIndex += waveform.ChannelNum)
            waveform.WaveData[SampleIndex] = Wavedata[SampleIndex];

        Waveforms.push_back(waveform);
    }

    catch (std::bad_alloc& alloc)
    {
        ModuleLogger::print("AddWaveform: out of memory (%s/%li)", WaveName.c_str(), WaveSize);
        return false;
    }
    catch (...)
    {
        ModuleLogger::print("unknown exception");
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
    Waveform waveform;

    try
    {
        // Load Resource
        hResource = FindResource(hInstance, MAKEINTRESOURCE(ResourceID), "RAW");
        bufferSize = SizeofResource(hInstance, hResource) / ByteSize;
        waveform.ChannelNum = ChannelNum;
        waveform.WaveSize = bufferSize;
        waveform.WaveName.assign(WaveName);
        waveform.WaveData = new double[waveform.WaveSize];

        data = waveform.WaveData;
        hRData = LoadResource(hInstance, hResource);

        if ((!data) || (!hRData))
            return false;

        pRData = (VstInt16 *)LockResource(hRData);
        UnlockResource(hRData);
        FreeResource(hRData);

        buf = pRData;
        Divisor = 1 << (ByteSize * 8);

        for (VstInt32 SampleIndex = 0; SampleIndex < waveform.WaveSize / waveform.ChannelNum; SampleIndex += waveform.ChannelNum)
            data[SampleIndex] = ((double)(buf[SampleIndex])) / Divisor;

        Waveforms.push_back(waveform);
    }

    catch (std::bad_alloc& alloc)
    {
        ModuleLogger::print("AddWaveform: out of memory (%s)", WaveName.c_str());
        return false;
    }
    catch (...)
    {
        ModuleLogger::print("unknown exception");
        return false;
    }
    return true;
}
#endif

double BaseWavetable::getWaveData(VstInt32 WaveIndex, double dPhase)
{
    double data = 0.0;

    if ((Waveforms.size() == 0) || (Waveforms[WaveIndex].WaveData == 0) || (WaveIndex >= Waveforms.size()))
        return data;

    data = Waveforms[WaveIndex].WaveData[(VstInt32)(dPhase * 2) % Waveforms[WaveIndex].WaveSize];
    data = ModuleHelper::clamp(data, -1.0, 1.0);
    return data;
}

bool BaseWavetable::getWaveName(VstInt32 WaveIndex, char* name)
{
    if (!name)
        return false;

    strcpy(name, Waveforms[WaveIndex].WaveName.c_str());
    return true;
}

long BaseWavetable::getWaveSize(VstInt16 WaveIndex)
{
    return Waveforms[WaveIndex].WaveSize;
}

double BaseWavetable::adjustPhase(VstInt16 WaveIndex, double phase)
{
    double dCorrect = (double)(getWaveSize(WaveIndex));
    if (phase >= dCorrect)
        return phase - dCorrect;
    else
        return phase;
}
