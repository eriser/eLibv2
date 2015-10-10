#include <Effect/Dither.h>

using namespace eLibV2::Effect;

void Dither::Init(void)
{
    setBitsize(DITHER_BITSIZE_DEFAULT);
    setBypass(false);
}

void Dither::Reset(void)
{
    Init();
}

double Dither::Process(const double Input)
{
    double Scaler;

    // input is between -1.0 and 1.0 with 16 bit resolution
    // which means an integer between -32768 and 32767

    // to reach a dithering effect this value is multiplied
    // by the dithering factor (2 ^ Bitsize) and cast to a long
    // resulting in -(2 ^ Bitsize) to (2 ^ Bitsize)
    // after that, the value is cast back to a double and
    // divided by the same dithering factor
    Scaler = pow(2.0, (int)m_uiBitsize);
    return ((double)((long)(Input * Scaler)) / Scaler);
}

double Dither::processConnection()
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(DITHER_CONNECTION_BYPASS))
        m_bBypass = ModuleHelper::double2bool(inputConnections[DITHER_CONNECTION_BYPASS]->processConnection(), 0.5);
    if (isInputConnected(DITHER_CONNECTION_BITSIZE))
        setBitsize((unsigned int)inputConnections[DITHER_CONNECTION_BITSIZE]->processConnection());
    if (isInputConnected(DITHER_CONNECTION_INPUT))
        dInput = inputConnections[DITHER_CONNECTION_INPUT]->processConnection();

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;

    // ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf -> %lf", getModuleName().c_str(), dInput, dOutput);
    return dOutput;
}
