/*
 * modOverdrive.cpp
 *
 *  Created on: 06.01.2012
 *      Author: dedokter
 */

#include <Effect/modOverdrive.h>

using namespace eLibV2;

FxOverdrive::FxOverdrive()
{
    Init();
}

void FxOverdrive::Init()
{
    setModuleName("Overdrive");

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
