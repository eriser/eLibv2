/*
 * modMixer.cpp
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#include "modMixer.h"

using namespace eLibV2;

FxMixer::FxMixer()
{
    Init();
}

void FxMixer::Init(void)
{
    ModuleName.assign("FxMixer");
    setVolume(0.0);
    setMode(MODE_MIXER_ADD);
}

void FxMixer::Reset(void)
{
    Init();
}

double FxMixer::Process2(double Input1, double Input2)
{
    double res = 0.0;

    switch (lMode)
    {
        case MODE_MIXER_ADD:
            res = Input1 + Input2;
            break;

        case MODE_MIXER_SUBTRACT:
            res = Input1 - Input2;
            break;

        case MODE_MIXER_INVSUBTRACT:
            res = Input2 - Input1;
            break;
    }

    return res;
}

void FxMixer::Test(void)
{
    
}
