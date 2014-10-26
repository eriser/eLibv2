/*
 * modSignalAdd.cpp
 *
 *  Created on: 13.01.2012
 *      Author: dedokter
 */

#include <Effect/modSignalAdd.h>

using namespace eLibV2;

FxSignalAdd::FxSignalAdd()
{
    Init();
}

void FxSignalAdd::Init(void)
{
    setModuleName("FxSignalAdd");
}

void FxSignalAdd::Reset(void)
{
    Init();
}

double FxSignalAdd::Process(double Input1, double Input2)
{
    return (Input1 + Input2) + (Input1 * Input2);
}

void FxSignalAdd::Test(void)
{
    char temp[1024];
    double In1, In2, Out;

    TestBeginMsg();
    for (long ii = 0; ii < 10; ii++)
    {
        In1 = GenerateTestSignal();
        In2 = GenerateTestSignal();
        Out = Process(In1, In2);
        sprintf(temp, "in1: %lf in2: %lf out: %lf", In1, In2, Out);
        dbgOutput(temp);
    }
    TestEndMsg();
}
