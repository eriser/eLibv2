/*
 * modWaveWrap.cpp
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#include "modWaveWrap.h"

using namespace eLibV2;

FxWaveWrap::FxWaveWrap()
{
    Init();
}

void FxWaveWrap::Init()
{
    ModuleName.assign("FxWaveWrap");
    // Positive Triangle
    for (long i = 0; i < WRAP_WAVESIZE; i++)
    {
        triangle[i] = (double)(i < (WRAP_WAVESIZE / 2)) ? ((double)(2.0 / (((double)WRAP_WAVESIZE / 2))) * i - 1.0) : (double)((double)(-2.0 / (((double)WRAP_WAVESIZE / 2))) * (i - (double)(WRAP_WAVESIZE / 2)) + 1.0);
    }
    setWrapLevel(0.0);
}

double FxWaveWrap::Process(double Input)
{
    double res = 0.0;
    double temp;

    temp = Input * triangle[((long)((Input + 1.0) * 32 * WRAP_WAVESIZE * dWrapLevel)) & (WRAP_WAVESIZE - 1)];

    res = temp;
    if (temp > 1.0)
        res = 2.0 - temp;

    if (temp < -1.0)
        res = -2.0 - temp;

    return res;
}
