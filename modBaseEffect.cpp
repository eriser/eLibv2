/*
 * modBaseEffect.cpp
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#include <iostream>
#include "modBaseEffect.h"

using namespace eLibV2;
using namespace std;

void BaseEffect::Init(void)
{
    ModuleName.assign("BaseEffect");
}

void BaseEffect::Reset(void)
{
    ModuleName.assign("BaseEffect resetted");
}

double BaseEffect::Process(double input)
{
    cout << "name: " << ModuleName << endl;
    return input * 2.1 * EULER;
}
