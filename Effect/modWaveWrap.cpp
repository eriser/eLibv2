#include <Effect/modWaveWrap.h>

/*
      ^
     / \
0  --------
   /     \
  /       \

  input: -1.0 ... 1.0
  wraplevel: 0.0 ... 1.0 (0.5)
  offset: 0 * 32 * 32768 * 0.5 ... 2 * 32 * 32768 * 0.5
  (input + 1.0) * 32 * WRAP_WAVESIZE * wraplevel

  temp = Input * triangle[((long)((Input + 1.0) * 32 * WRAP_WAVESIZE * dWrapLevel)) & (WRAP_WAVESIZE - 1)];

*/
void FxWaveWrap::Init()
{
    // Positive Triangle
    for (long i = 0; i < WRAP_WAVESIZE; i++)
        triangle[i] = (double)(i < (WRAP_WAVESIZE / 2)) ? ((double)(2.0 / (((double)WRAP_WAVESIZE / 2))) * i - 1.0) : (double)((double)(-2.0 / (((double)WRAP_WAVESIZE / 2))) * (i - (double)(WRAP_WAVESIZE / 2)) + 1.0);
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

double FxWaveWrap::processIOs()
{
	double input = 0.0;

	if (isAttached(WAVEWRAP_LEVEL))
		setWrapLevel(controlIOs[WAVEWRAP_LEVEL]->processIOs());
	else if (isAttached(WAVEWRAP_INPUT))
		input = controlIOs[WAVEWRAP_INPUT]->processIOs();
	ModuleLogger::print("%s::process %lf", getModuleName().c_str(), input);

	return Process(input);
}

bool FxWaveWrap::Test()
{
	for (long i = 0; i < WRAP_WAVESIZE; i++)
	{
		if (i % 256 == 0)
			ModuleLogger::print("Triangle: %li %lf", i, triangle[i]);
	}
	return true;
}