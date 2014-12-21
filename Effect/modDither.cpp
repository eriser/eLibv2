#include <Effect/modDither.h>

void FxDither::setBitsize(VstInt16 Bitsize)
{
    if ((Bitsize >= DITHER_BITSIZE_MIN) && (Bitsize <= DITHER_BITSIZE_MAX))
        mBitsize = Bitsize;
    else
        dbgOutputF("bitsize out of range: %li -> using default (%li)", Bitsize, mBitsize);
}

void FxDither::Init(void)
{
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
    // by the dithering factor (2 ^ Bitsize) and cast to a long
    // resulting in -(2 ^ Bitsize) to (2 ^ Bitsize)
    // after that, the value is cast back to a double and
    // divided by the same dithering factor
    Scaler = pow(2.0, (int)mBitsize);
    return ((double)((long)(Input * Scaler)) / Scaler);
}

double FxDither::processIOs()
{
	double input = 0.0;

	if (isAttached(DITHER_BITSIZE))
		setBitsize((unsigned int)controlIOs[DITHER_BITSIZE]->processIOs());
	if (isAttached(DITHER_INPUT))
		input = controlIOs[DITHER_INPUT]->processIOs();
	ModuleLogger::print("%s::process %lf", getModuleName().c_str(), input);

	return Process(input);
}

bool FxDither::Test(void)
{
    double In, Out;

    TestBeginMsg();
    for (VstInt16 bitsize = 16; bitsize >= 2; bitsize--)
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

	return true;
}
