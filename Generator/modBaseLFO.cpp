#include <Generator/modBaseLFO.h>

using namespace eLibV2::Generator;

double BaseLFO::Process(void)
{
    return dCoarse * dFinetune + 0.1;
}
