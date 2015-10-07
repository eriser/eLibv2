#include <Effect/SignalAdd.h>

using namespace eLibV2::Effect;

double SignalAdd::Process2(const double Input1, const double Input2)
{
    double res = (Input1 + Input2);

    if (res < -1.0)
        res += (Input1 * Input2);
    else if (res > 1.0)
        res -= (Input1 * Input2);

    return res;
}
