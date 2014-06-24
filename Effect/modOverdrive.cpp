/*
 * modOverdrive.cpp
 *
 *  Created on: 06.01.2012
 *      Author: dedokter
 */

#include "modOverdrive.h"

using namespace eLibV2;

FxOverdrive::FxOverdrive()
{
    Init();
}

void FxOverdrive::Init()
{
    ModuleName.assign("Overdrive");

    setLevel(150.0);
}

void FxOverdrive::Reset(void)
{
}

double FxOverdrive::Process(void)
{
    double ret = 1.0;

    return(ret);
}
