#include <Effect/modSignalAdd.h>

using namespace eLibV2::Effect;

double FxSignalAdd::Process(double Input1, double Input2)
{
    return (Input1 + Input2) + (Input1 * Input2);
}
