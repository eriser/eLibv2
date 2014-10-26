/*
 * modWaveWrap.cpp
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#include <Generator/modNoiseGenerator.h>

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
    setModuleName("ToneGenerator");
}

double NoiseGenerator::Process(void)
{
    return GenerateTestSignal();
}
