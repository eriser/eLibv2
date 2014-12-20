/*
 * mod3BandEQ.cpp
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#include <Effect/mod3BandEQ.h>

#include <math.h>
#include <memory.h>

using namespace eLibV2;

Fx3BandEQ::Fx3BandEQ()
{
    Init();
}

void Fx3BandEQ::Init()
{
    setModuleName("3BandEQ");

    setGain(0, 0.0);
    setGain(1, 0.0);
    setGain(2, 0.0);
    setGain(3, 0.0);
    setFrequency(0, 200.0);
    setFrequency(1, 1000.0);
    setFrequency(2, 2000.0);
    setSamplerate(44100.0);

    buffer[0] = buffer[1] = buffer[2] = 0.0;
}

void Fx3BandEQ::Reset(void)
{
}

void Fx3BandEQ::setSamplerate(double Samplerate)
{
    dSamplerate = Samplerate;

    for (VstInt16 FreqIndex = 0; FreqIndex < (EQ_NUM_BANDS - 1); FreqIndex++)
        setFrequency(FreqIndex, dFrequency[FreqIndex]);
}

void Fx3BandEQ::setFrequency(VstInt16 Index, double Frequency)
{
    memset(&Bands[Index], 0, sizeof(Bands[Index]));
    dFrequency[Index] = Frequency;
    Bands[Index].Frequency = 2 * sin(PI * (dFrequency[Index] / dSamplerate));
}

double Fx3BandEQ::CalcBand(VstInt16 BandIndex, double Input)
{
    double Freq = Bands[BandIndex].Frequency;

    Bands[BandIndex].Poles[0] += (Freq * (Input - Bands[BandIndex].Poles[0])) + vsa;
    Bands[BandIndex].Poles[1] += (Freq * (Bands[BandIndex].Poles[0] - Bands[BandIndex].Poles[1]));
    Bands[BandIndex].Poles[2] += (Freq * (Bands[BandIndex].Poles[1] - Bands[BandIndex].Poles[2]));
    Bands[BandIndex].Poles[3] += (Freq * (Bands[BandIndex].Poles[2] - Bands[BandIndex].Poles[3]));
    return Bands[BandIndex].Poles[3];
}

double Fx3BandEQ::Process(double Input)
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
