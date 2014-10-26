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
    setModuleName("BaseEffect");
}

void BaseEffect::Reset(void)
{
    setModuleName("BaseEffect resetted");
}

double BaseEffect::Process(double input)
{
    cout << "name: " << getModuleName() << endl;
    return input * 2.1 * EULER;
}
