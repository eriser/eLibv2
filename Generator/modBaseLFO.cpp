#include <Generator/modBaseLFO.h>

double BaseLFO::Process(void)
{
    return dCoarse * dFinetune + 0.1;
}
