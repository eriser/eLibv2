/*
 * modBaseLFO.cpp
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#include <Generator/modBaseLFO.h>

using namespace eLibV2;

void BaseLFO::Init(void)
{
    setModuleName("BaseLFO");
}

double BaseLFO::Process(void)
{
    return dCoarse * dFinetune + 0.1;
}
