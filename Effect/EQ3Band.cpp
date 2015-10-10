#include <Effect/EQ3Band.h>

using namespace eLibV2::Effect;

void EQ3Band::Init()
{
    setGain(0, 0.0);
    setGain(1, 0.0);
    setGain(2, 0.0);
    setGain(3, 0.0);
    setFrequency(0, 200.0);
    setFrequency(1, 1000.0);
    setFrequency(2, 2000.0);
    setSamplerate(44100.0);

    memset(buffer, 0, sizeof(buffer));
}

void EQ3Band::Reset(void)
{
}

void EQ3Band::setSamplerate(const double Samplerate)
{
    mSamplerate = Samplerate;

    for (VstInt16 FreqIndex = 0; FreqIndex < (EQ_NUM_BANDS - 1); FreqIndex++)
        setFrequency(FreqIndex, dFrequency[FreqIndex]);
}

void EQ3Band::setFrequency(VstInt16 Index, double Frequency)
{
    memset(&Bands[Index], 0, sizeof(Bands[Index]));
    dFrequency[Index] = Frequency;
    Bands[Index].Frequency = 2 * sin(PI * (dFrequency[Index] / mSamplerate));
}

double EQ3Band::CalcBand(VstInt16 BandIndex, double Input)
{
    double Freq = Bands[BandIndex].Frequency;

    Bands[BandIndex].Poles[0] += (Freq * (Input - Bands[BandIndex].Poles[0])) + mVSA;
    Bands[BandIndex].Poles[1] += (Freq * (Bands[BandIndex].Poles[0] - Bands[BandIndex].Poles[1]));
    Bands[BandIndex].Poles[2] += (Freq * (Bands[BandIndex].Poles[1] - Bands[BandIndex].Poles[2]));
    Bands[BandIndex].Poles[3] += (Freq * (Bands[BandIndex].Poles[2] - Bands[BandIndex].Poles[3]));
    return Bands[BandIndex].Poles[3];
}

double EQ3Band::Process(const double Input)
{
    double low, mid1, mid2, high;

    low = CalcBand(0, Input);
    mid1 = CalcBand(1, Input);
    mid2 = CalcBand(2, Input);
    high = buffer[EQ_BUFFER_SIZE - 1] - CalcBand(3, Input);

    low *= dGain[0];
    mid1 *= dGain[1];
    mid2 *= dGain[2];
    high *= dGain[3];

    for (VstInt16 BufferIndex = EQ_BUFFER_SIZE - 1; BufferIndex >= 0; BufferIndex--)
        buffer[BufferIndex] = (BufferIndex) ? buffer[BufferIndex - 1] : Input;

    return(mid1 + mid2 + high);
}

double EQ3Band::processConnection()
{
    double input = 0.0;

    if (isInputConnected(0))
        input = inputConnections[0]->processConnection();
    ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

    return Process(input);
}
