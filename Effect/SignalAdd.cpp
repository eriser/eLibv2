#include <Effect/SignalAdd.h>

using namespace eLibV2::Effect;

double FxSignalAdd::Process(double Input1, double Input2)
{
    double res = (Input1 + Input2);

    if (res < -1.0)
        res += (Input1 * Input2);
    else if (res > 1.0)
        res -= (Input1 * Input2);

    return res;
}
