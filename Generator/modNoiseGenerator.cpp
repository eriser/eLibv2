/*
 * modWaveWrap.cpp
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#include "modNoiseGenerator.h"

using namespace eLibV2;

NoiseGenerator::NoiseGenerator()
{
    Init();
}

NoiseGenerator::~NoiseGenerator()
{
}

void NoiseGenerator::Init()
{
    ModuleName.assign("ToneGenerator");
}

double NoiseGenerator::Process(void)
{
    return GenerateTestSignal();
}
