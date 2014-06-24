/*
 * modBaseLFO.cpp
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#include "modBaseLFO.h"

using namespace eLibV2;

void BaseLFO::Init(void)
{
    ModuleName.assign("BaseLFO");
}

double BaseLFO::Process(void)
{
    return dCoarse * dFinetune + 0.1;
}
