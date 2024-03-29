#include <Effect/WaveWrap.h>

using namespace eLibV2::Effect;

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

  temp = Input * triangle[((SInt32)((Input + 1.0) * 32 * WRAP_WAVESIZE * dWrapLevel)) & (WRAP_WAVESIZE - 1)];

*/
void WaveWrap::Init()
{
    // Positive Triangle
    triangle = new double[WRAP_WAVESIZE];
    for (SInt32 i = 0; i < WRAP_WAVESIZE; i++)
        triangle[i] = (double)(i < (WRAP_WAVESIZE / 2)) ? ((double)(2.0 / (((double)WRAP_WAVESIZE / 2))) * i - 1.0) : (double)((double)(-2.0 / (((double)WRAP_WAVESIZE / 2))) * (i - (double)(WRAP_WAVESIZE / 2)) + 1.0);
    setWrapLevel(1.0);
}

double WaveWrap::Process(const double Input)
{
    double res = 0.0;
    double temp;

    temp = Input * triangle[((SInt32)((Input + 1.0) * 32 * WRAP_WAVESIZE * dWrapLevel)) & (WRAP_WAVESIZE - 1)];

    res = temp;
    if (temp > 1.0)
        res = 2.0 - temp;

    if (temp < -1.0)
        res = -2.0 - temp;

    return res;
}

double WaveWrap::processConnection(void)
{
    double input = 0.0;

    if (isInputConnected(WAVEWRAP_CONNECTION_INPUT))
        input = inputConnections[WAVEWRAP_CONNECTION_INPUT]->processConnection();
    if (isInputConnected(WAVEWRAP_CONNECTION_LEVEL))
        setWrapLevel(inputConnections[WAVEWRAP_CONNECTION_LEVEL]->processConnection());
    ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

    return Process(input);
}
