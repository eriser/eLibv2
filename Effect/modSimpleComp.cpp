/*
 * modSimpleComp.cpp
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#include <Effect/modSimpleComp.h>

#include <math.h>

using namespace eLibV2;

FxSimpleComp::FxSimpleComp()
{
    Init();
}

void FxSimpleComp::Init()
{
    setModuleName("FxSimpleComp");

    dThreshold = 1.0;
    dAttack = 0.0;
    dRelease = 0.0;
    dRatio = 0.0;
    dOutput = 1.0;

    dEnvDecay = 0.0;

    dTransferA = 0.0;
    dTransferB = 1.0;

    Reset();
}

void FxSimpleComp::Reset()
{
    dEnv = 0.0;
    dGain = 1.0;
}

void FxSimpleComp::setThreshold(double Threshold)
{
    dThreshold = Threshold;
    dTransferB = dOutput * pow(dThreshold,-dTransferA);
}

void FxSimpleComp::setRatio(double Ratio)
{
    dRatio = Ratio;
    dTransferA = Ratio - 1.0;
    dTransferB = dOutput * pow(dThreshold,-dTransferA);
}

void FxSimpleComp::setAttack(double Attack)
{
    dAttack = exp(-1.0 / Attack);
}

void FxSimpleComp::setRelease(double Release)
{
    dRelease = exp(-1.f / Release);
    dEnvDecay = exp(-4.f / Release); /* = exp(-1/(0.25*value)) */
}

void FxSimpleComp::setOutput(double Output)
{
    dOutput = Output;
    dTransferB = dOutput * pow(dThreshold,-dTransferA);
}

double FxSimpleComp::Process(double Input)
{
    double det, transfer_gain, res = 0.0;

    det = fabs(Input);
    det += 10e-30f; /* add tiny DC offset (-600dB) to prevent denormals */

    dEnv = det >= dEnv ? det : det + dEnvDecay * (dEnv - det);

    transfer_gain = dEnv > dThreshold ? pow(dEnv, dTransferA) * dTransferB : dOutput;

    dGain = transfer_gain < dGain ? transfer_gain + dAttack * (dGain - transfer_gain) : transfer_gain + dRelease * (dGain - transfer_gain);

    res = Input * dGain;

    return res;
}
