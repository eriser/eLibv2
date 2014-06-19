/*
 * modDither.cpp
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#include <math.h>
#include "modDither.h"

using namespace eLibV2;

FxDither::FxDither(void)
{
    Init();
}

void FxDither::setBitsize(long Bitsize)
{
    if ((Bitsize >= DITHER_BITSIZE_MIN) && (Bitsize <= DITHER_BITSIZE_MAX))
        lBitsize = Bitsize;
    else
        dbgOutputF("bitsize out of range: %li -> using %li", Bitsize, lBitsize);
}

void FxDither::Init(void)
{
    ModuleName.assign("FxDither");
    setBitsize(12);
}

void FxDither::Reset(void)
{
    Init();
}

double FxDither::Process(double Input)
{
    double Scaler;

    // input is between -1.0 and 1.0 with 16 bit resolution
    // which means an integer between -32768 and 32767

    // to reach a dithering effect this value is multiplied
    // by the dithering factor (2 ^ Bitsize) and cast a long
    // resulting in -(2 ^ Bitsize) to (2 ^ Bitsize)
    // after this the value is cast back to a double and
    // divided by the same dithering factor
    Scaler = pow(2.0, lBitsize);
    return ((double)((long)(Input * Scaler)) / Scaler);
}

void FxDither::Test(void)
{
    double In, Out;

    TestBeginMsg();
    for (long bitsize = 16; bitsize >= 2; bitsize--)
    {
        dbgOutputF("setting bitsize to: %li", bitsize);
        setBitsize(bitsize);
        for (long ii = 0; ii < 10; ii++)
        {
            In = GenerateTestSignal();
            Out = Process(In);
            dbgOutputF("bitsize: %li in: %lf out: %lf", bitsize, In, Out);
        }
    }
    TestEndMsg();
}
