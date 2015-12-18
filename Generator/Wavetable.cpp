#include <Generator/Wavetable.h>

using namespace eLibV2::Generator;
using namespace eLibV2::Loader;
using namespace eLibV2::Util;

BaseWavetable* BaseWavetable::m_pInstance = 0;
UInt32 BaseWavetable::m_lWaveSize = SAMPLE_WAVESIZE;
UInt8 BaseWavetable::m_iBandLimit = SAMPLE_BANDLIMIT;

BaseWavetable::~BaseWavetable()
{
    for (UInt16 WaveformIndex = 0; WaveformIndex < Waveforms.size(); WaveformIndex++)
        DeleteWaveform(WaveformIndex);
}

BaseWavetable* BaseWavetable::getInstance()
{
    if (!m_pInstance)
        m_pInstance = new BaseWavetable();
    return m_pInstance;
}

void BaseWavetable::Init()
{
    // prepare random coefficients
    double random = 0.0, sh_random = 0.0;
    const static SInt16 q = 15;
    const static double c1 = (1 << q) - 1;
    const static double c2 = (double)(((SInt16)(c1 / 3)) + 1);
    const static double c3 = 1.0 / c1;

    // add empty waveform
    Waveform waveform = { 0 };
    Waveforms.push_back(waveform);

    // builtin waveforms
    double *sine;
    double *enis;
    double *triangle_bl;
    double triangle_bl_max = 0.0;
    double *sawup_bl;
    double sawup_bl_max = 0.0;
    double *sawdn_bl;
    double sawdn_bl_max = 0.0;
    double *pulse_bl;
    double pulse_bl_max = 0.0;

    double *triangle;
    double *sawup;
    double *sawdn;
    double *pulse;
    double *noise;
    double *smplhold;

    srand((unsigned)time(NULL));

    try
    {
        // bandlimited
        sine = new double[m_lWaveSize];
        enis = new double[m_lWaveSize];
        triangle_bl = new double[m_lWaveSize];
        sawup_bl = new double[m_lWaveSize];
        sawdn_bl = new double[m_lWaveSize];
        pulse_bl = new double[m_lWaveSize];

        // digital
        triangle = new double[m_lWaveSize];
        sawup = new double[m_lWaveSize];
        sawdn = new double[m_lWaveSize];
        pulse = new double[m_lWaveSize];
        noise = new double[m_lWaveSize];
        smplhold = new double[m_lWaveSize];

        // create waveforms
        for (UInt32 SampleIndex = 0; SampleIndex < m_lWaveSize; ++SampleIndex)
        {
            // bandlimited waveforms
            sine[SampleIndex] = sin((2.0 * PI * ((double)SampleIndex / (double)m_lWaveSize)));
            enis[SampleIndex] = -sine[SampleIndex];

            /* generate triangle waveform (0.0 -> 1.0, 1.0 -> -1.0, -1.0 -> 0.0 */
            triangle_bl[SampleIndex] = 0.0;
            for (UInt8 WaveIndex = 0; WaveIndex <= (m_iBandLimit + 1) / 2; ++WaveIndex)
            {
                triangle_bl[SampleIndex] += pow(-1.0, WaveIndex) * (1.0 / pow((2 * WaveIndex + 1), 2.0)) * sin(2.0 * PI * (2.0 * WaveIndex + 1) * (SampleIndex / (double)m_lWaveSize));
                if (triangle_bl[SampleIndex] > triangle_bl_max)
                    triangle_bl_max = triangle_bl[SampleIndex];
            }

            sawup_bl[SampleIndex] = 0.0;
            for (UInt8 WaveIndex = 1; WaveIndex <= (m_iBandLimit + 1); ++WaveIndex)
            {
                sawup_bl[SampleIndex] += pow(-1.0, (double)(WaveIndex + 1)) * (1.0 / (double)WaveIndex) * sin(2.0 * PI * (double)SampleIndex * (double)WaveIndex / (double)m_lWaveSize);
                if (sawup_bl[SampleIndex] > sawup_bl_max)
                    sawup_bl_max = sawup_bl[SampleIndex];
            }

            sawdn_bl[SampleIndex] = 0.0;
            for (UInt8 WaveIndex = 1; WaveIndex <= (m_iBandLimit + 1); ++WaveIndex)
            {
                sawdn_bl[SampleIndex] += (1.0 / (double)WaveIndex) * sin(2.0 * PI * (double)SampleIndex * (double)WaveIndex / (double)m_lWaveSize);
                if (sawdn_bl[SampleIndex] > sawdn_bl_max)
                    sawdn_bl_max = sawdn_bl[SampleIndex];
            }

            pulse_bl[SampleIndex] = 0.0;
            for (UInt8 WaveIndex = 1; WaveIndex <= m_iBandLimit; WaveIndex += 2)
            {
                pulse_bl[SampleIndex] += (1.0 / (double)WaveIndex) * sin(2.0 * PI * (double)SampleIndex * (double)WaveIndex / (double)m_lWaveSize);
                if (pulse_bl[SampleIndex] > pulse_bl_max)
                    pulse_bl_max = pulse_bl[SampleIndex];
            }

            // digital waveforms (including aliasing)
            /* generate triangle waveform (0.0 -> 1.0, 1.0 -> -1.0, -1.0 -> 0.0 */
            if (SampleIndex <= m_lWaveSize / 4)
                triangle[SampleIndex] = 1.0 / (m_lWaveSize / 4) * SampleIndex;
            if ((SampleIndex <= 3 * m_lWaveSize / 4) && (SampleIndex > m_lWaveSize / 4))
                triangle[SampleIndex] = -2.0 / (m_lWaveSize / 2) * (SampleIndex - m_lWaveSize / 4) + 1.0;
            if (SampleIndex > 3 * m_lWaveSize / 4)
                triangle[SampleIndex] = 1.0 / (m_lWaveSize / 4) * (SampleIndex - 3 * m_lWaveSize / 4) - 1.0;

            /* generate rising sawtooth waveform (0.0 -> 1.0, -1.0 -> 0.0) */
            if (SampleIndex < m_lWaveSize / 2)
                sawup[SampleIndex] = 1.0 / (m_lWaveSize / 2) * SampleIndex;
            else if (SampleIndex == m_lWaveSize / 2)
                sawup[SampleIndex] = 0.0;
            else
                sawup[SampleIndex] = 1.0 / (m_lWaveSize / 2) * (SampleIndex - m_lWaveSize / 2) - 1.0;

            /* generate falling sawtooth waveform (0.0 -> -1.0, 1.0 -> 0.0) */
            if (SampleIndex < m_lWaveSize / 2)
                sawdn[SampleIndex] = -1.0 / (m_lWaveSize / 2) * SampleIndex;
            else if (SampleIndex == m_lWaveSize / 2)
                sawdn[SampleIndex] = 0.0;
            else
                sawdn[SampleIndex] = -1.0 / (m_lWaveSize / 2) * (SampleIndex - m_lWaveSize / 2) + 1.0;

            /* generate pulse waveform */
            if ((SampleIndex == 0) || (SampleIndex == m_lWaveSize / 2))
                pulse[SampleIndex] = 0.0f;
            else
                pulse[SampleIndex] = (SampleIndex < (m_lWaveSize / 2)) ? 1.0 : -1.0;

            random = ((double)rand() / ((double)RAND_MAX) + 1.0);
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

        // normalize bandlimited waveforms
        for (UInt32 SampleIndex = 0; SampleIndex < m_lWaveSize; ++SampleIndex)
        {
            triangle_bl[SampleIndex] /= triangle_bl_max;
            sawup_bl[SampleIndex] /= sawup_bl_max;
            sawdn_bl[SampleIndex] /= sawdn_bl_max;
            pulse_bl[SampleIndex] /= pulse_bl_max;
        }

        AddWaveform(sine, m_lWaveSize, "sine");
        if (sine)
            delete[] sine;

        AddWaveform(enis, m_lWaveSize, "enis");
        if (enis)
            delete[] enis;

        AddWaveform(triangle_bl, m_lWaveSize, "tribl");
        if (triangle_bl)
            delete[] triangle_bl;

        AddWaveform(sawup_bl, m_lWaveSize, "sawupbl");
        if (sawup_bl)
            delete[] sawup_bl;

        AddWaveform(sawdn_bl, m_lWaveSize, "sawdnbl");
        if (sawdn_bl)
            delete[] sawdn_bl;

        AddWaveform(pulse_bl, m_lWaveSize, "pulsebl");
        if (pulse_bl)
            delete[] pulse_bl;

        AddWaveform(triangle, m_lWaveSize, "tri");
        if (triangle)
            delete[] triangle;

        AddWaveform(sawup, m_lWaveSize, "sawup");
        if (sawup)
            delete[] sawup;

        AddWaveform(sawdn, m_lWaveSize, "sawdn");
        if (sawdn)
            delete[] sawdn;

        AddWaveform(pulse, m_lWaveSize, "pulse");
        if (pulse)
            delete[] pulse;

        AddWaveform(noise, m_lWaveSize, "noise");
        if (noise)
            delete[] noise;

        AddWaveform(smplhold, m_lWaveSize, "smhld");
        if (smplhold)
            delete[] smplhold;
    }
    catch (std::bad_alloc& alloc)
    {
        // reaching here lacks WAVEFORM_BUILTIN_NUM * kWavesize bytes of memory
        ModuleLogger::print(LOG_CLASS_GENERATOR, "Init: out of memory");
    }
    catch (...)
    {
        ModuleLogger::print(LOG_CLASS_GENERATOR, "Init: unknown exception");
    }
}

void BaseWavetable::DeleteWaveform(UInt16 Index)
{
    if (Waveforms[Index].WaveData)
    {
        delete Waveforms[Index].WaveData;
        Waveforms[Index].WaveData = NULL;
    }
}

bool BaseWavetable::AddWaveform(const std::string Filename, const std::string WaveName)
{
    WaveLoader waveLoader;
    WaveLoader::WaveFormat waveFormat;
    Waveform waveform;
    float *waveData;

    try
    {
        if (waveLoader.Load(Filename))
            return false;

        waveFormat = waveLoader.getWaveFormat();
        waveform.ChannelNum = waveFormat.NumChannels;
        waveform.WaveSize = waveLoader.getWaveSize();
        waveform.WaveName.assign(WaveName);
        waveform.WaveData = new double[waveform.WaveSize];

        // copy wave data
        if (waveLoader.getWaveFormat().NumChannels > 1)
            ModuleLogger::print(LOG_CLASS_GENERATOR, "loaded multichannel file.");
        waveData = waveLoader.getWaveData(0);
        if (!waveData)
            return false;

        // get only data of first channel
        for (UInt32 SampleIndex = 0; SampleIndex < waveform.WaveSize / waveform.ChannelNum; SampleIndex += waveform.ChannelNum)
            waveform.WaveData[SampleIndex] = ((double)waveData[SampleIndex]);

        waveLoader.Unload();
        Waveforms.push_back(waveform);
    }
    catch (std::bad_alloc& alloc)
    {
        ModuleLogger::print(LOG_CLASS_GENERATOR, "AddWaveform: out of memory (%s)", WaveName.c_str());
        return false;
    }
    catch (...)
    {
        ModuleLogger::print(LOG_CLASS_GENERATOR, "unknown exception");
        return false;
    }
    return true;
}

bool BaseWavetable::AddWaveform(const double *Wavedata, const UInt32 WaveSize, const std::string WaveName, const UInt8 ChannelNum)
{
    Waveform waveform;

    try
    {
        ModuleLogger::print(LOG_CLASS_GENERATOR, "AddWaveform: %s/%li", WaveName.c_str(), WaveSize);

        waveform.ChannelNum = ChannelNum;
        waveform.WaveSize = WaveSize;
        waveform.WaveName.assign(WaveName);
        waveform.WaveData = new double[waveform.WaveSize];

        // copy wave data
        if (!Wavedata)
            return false;

        for (UInt32 SampleIndex = 0; SampleIndex < waveform.WaveSize / waveform.ChannelNum; SampleIndex += waveform.ChannelNum)
            waveform.WaveData[SampleIndex] = Wavedata[SampleIndex];

        Waveforms.push_back(waveform);
    }

    catch (std::bad_alloc& alloc)
    {
        ModuleLogger::print(LOG_CLASS_GENERATOR, "AddWaveform: out of memory (%s/%li)", WaveName.c_str(), WaveSize);
        return false;
    }
    catch (...)
    {
        ModuleLogger::print(LOG_CLASS_GENERATOR, "unknown exception");
        return false;
    }
    return true;
}

#if defined(WIN32)
bool BaseWavetable::AddWaveform(const HINSTANCE hInstance, const UInt16 ResourceID, const std::string WaveName, const UInt8 ByteSize, const UInt8 ChannelNum)
{
    HGLOBAL hRData = NULL;
    HRSRC hResource = NULL;
    double *data;
    SInt16 *pRData;
    SInt16 *buf;
    SInt32 bufferSize, Divisor;
    Waveform waveform;

    try
    {
        // Load Resource
        hResource = FindResource(hInstance, MAKEINTRESOURCE(ResourceID), "RAW");
        if (hResource)
        {
            bufferSize = SizeofResource(hInstance, hResource) / ByteSize;
            waveform.ChannelNum = ChannelNum;
            waveform.WaveSize = bufferSize;
            waveform.WaveName.assign(WaveName);
            waveform.WaveData = new double[waveform.WaveSize];

            data = waveform.WaveData;
            hRData = LoadResource(hInstance, hResource);

            if ((!data) || (!hRData))
                return false;

            pRData = (SInt16 *)LockResource(hRData);
            UnlockResource(hRData);
            FreeResource(hRData);

            buf = pRData;
            Divisor = 1 << (ByteSize * 8);

            for (SInt32 SampleIndex = 0; SampleIndex < waveform.WaveSize / waveform.ChannelNum; SampleIndex += waveform.ChannelNum)
                data[SampleIndex] = ((double)(buf[SampleIndex])) / Divisor;

            Waveforms.push_back(waveform);
        }
    }

    catch (std::bad_alloc& alloc)
    {
        ModuleLogger::print(LOG_CLASS_GENERATOR, "AddWaveform: out of memory (%s)", WaveName.c_str());
        return false;
    }
    catch (...)
    {
        ModuleLogger::print(LOG_CLASS_GENERATOR, "unknown exception");
        return false;
    }
    return true;
}

double* BaseWavetable::loadWaveform(const HINSTANCE hInstance, const UInt16 resID, double *data, const UInt8 ByteSize, const UInt8 ChannelNum)
{
    HGLOBAL hResourceData = NULL;
    HRSRC hResource = NULL;
    short *pResourceData;
    SInt32 bufferSize;

    // Load Resource //
    hResource = FindResource(hInstance, MAKEINTRESOURCE(resID), "RAW");
    if (hResource)
    {
        if (((SInt32)SizeofResource(hInstance, hResource) / (ByteSize * ChannelNum)) > m_lWaveSize)
            bufferSize = m_lWaveSize;
        else
            bufferSize = SizeofResource(hInstance, hResource) / (ByteSize * ChannelNum);

        hResourceData = LoadResource(hInstance, hResource);
        if (hResourceData)
        {
            pResourceData = (short *)LockResource(hResourceData);
            UnlockResource(hResourceData);
            FreeResource(hResourceData);

            for (SInt32 i = 0; i < bufferSize; i++)
            {
                data[i] = ((double)(pResourceData[i])) / 0x10000;
            }
        }
    }
    return 0;
}
#endif

double BaseWavetable::getWaveData(const UInt16 WaveIndex, const double dPhase, const double PulseWidth)
{
    double data = 0.0;

    if ((Waveforms.size() == 0) || (Waveforms[WaveIndex].WaveData == 0) || (WaveIndex >= Waveforms.size()))
        return data;

    // prepare for linear interpolation
    SInt32 firstIndex = (SInt32)(dPhase * 2);
    double dFrac = (dPhase * 2) - firstIndex;

    firstIndex = ModuleHelper::wrap(firstIndex, Waveforms[WaveIndex].WaveSize);
    SInt32 nextIndex = ModuleHelper::wrap(firstIndex + 1, Waveforms[WaveIndex].WaveSize);

    // special treatment for pulse-width
    // use two sawtooth-waveform (up and down) with phase shift and subtract
    if ((WaveIndex == WAVEFORM_PULSE) || (WaveIndex == WAVEFORM_PULSE_BL))
    {
        // change waveforms to sawtooth
        UInt16 WaveIndex1, WaveIndex2;
        if (WaveIndex == WAVEFORM_PULSE)
        {
            WaveIndex1 = WAVEFORM_SAWUP;
            WaveIndex2 = WAVEFORM_SAWUP;
        }
        else
        {
            WaveIndex1 = WAVEFORM_SAWUP_BL;
            WaveIndex2 = WAVEFORM_SAWUP_BL;
        }

        // prepare phase shift for second sawtooth
        double dPhase2 = dPhase + PulseWidth * Waveforms[WaveIndex].WaveSize * 0.5;
        SInt32 firstIndex2 = (SInt32)(dPhase2 * 2);
        double dFrac2 = (dPhase2 * 2) - firstIndex2;

        firstIndex2 = ModuleHelper::wrap(firstIndex2, Waveforms[WaveIndex].WaveSize);
        SInt32 nextIndex2 = ModuleHelper::wrap(firstIndex2 + 1, Waveforms[WaveIndex].WaveSize);

        double data1 = ModuleHelper::LinearInterpolation(Waveforms[WaveIndex].WaveData[firstIndex], Waveforms[WaveIndex].WaveData[nextIndex], dFrac);
        double data2 = ModuleHelper::LinearInterpolation(Waveforms[WaveIndex].WaveData[firstIndex2], Waveforms[WaveIndex].WaveData[nextIndex2], dFrac2);
        data = 0.5 * data1 - 0.5 * data2;
    }
    else
    {
        // data = Waveforms[WaveIndex].WaveData[(SInt32)(dPhase * 2) % Waveforms[WaveIndex].WaveSize];
        data = ModuleHelper::LinearInterpolation(Waveforms[WaveIndex].WaveData[firstIndex], Waveforms[WaveIndex].WaveData[nextIndex], dFrac);
    }

    data = ModuleHelper::clamp(data, -1.0, 1.0);
    return data;
}

bool BaseWavetable::getWaveName(const UInt16 WaveIndex, char* name)
{
    if (!name)
        return false;

    strcpy(name, Waveforms[WaveIndex].WaveName.c_str());
    return true;
}

SInt32 BaseWavetable::getWaveSize(const UInt16 WaveIndex)
{
    return Waveforms[WaveIndex].WaveSize;
}

double BaseWavetable::adjustPhase(const UInt16 WaveIndex, const double phase)
{
    double dCorrect = (double)(getWaveSize(WaveIndex));
    if (phase >= dCorrect)
        return phase - dCorrect;
    else
        return phase;
}
