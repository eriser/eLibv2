#include <Effect/Dither.h>

using namespace eLibV2::Effect;

void FxDither::Init(void)
{
    setBitsize(12);
    setBypass(false);
}

void FxDither::Reset(void)
{
    Init();
}

double FxDither::Process(const double Input)
{
    double Scaler;

    // input is between -1.0 and 1.0 with 16 bit resolution
    // which means an integer between -32768 and 32767

    // to reach a dithering effect this value is multiplied
    // by the dithering factor (2 ^ Bitsize) and cast to a long
    // resulting in -(2 ^ Bitsize) to (2 ^ Bitsize)
    // after that, the value is cast back to a double and
    // divided by the same dithering factor
    Scaler = pow(2.0, (int)mBitsize);
    return ((double)((long)(Input * Scaler)) / Scaler);
}

double FxDither::processConnection()
{
    double input = 0.0;

    //if (inputConnections[DITHER_CONNECTION_BYPASS] != NULL)
    //   inputConnections[DITHER_CONNECTION_BYPASS]->processConnection();
    if (inputConnections[DITHER_CONNECTION_BITSIZE] != NULL)
        setBitsize((unsigned int)inputConnections[DITHER_CONNECTION_BITSIZE]->processConnection());
    if (inputConnections[DITHER_CONNECTION_INPUT])
        input = inputConnections[DITHER_CONNECTION_INPUT]->processConnection();
    // ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);
    if (!bBypass)
        return Process(input);
    else
        return input;
}
