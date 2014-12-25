#include <Effect/modSignalAdd.h>

double FxSignalAdd::Process(double Input1, double Input2)
{
    return (Input1 + Input2) + (Input1 * Input2);
}

bool FxSignalAdd::Test(void)
{
    double In1, In2, Out;

    TestBeginMsg();
    for (long ii = 0; ii < 10; ii++)
    {
        In1 = GenerateTestSignal();
        In2 = GenerateTestSignal();
        Out = Process(In1, In2);
		ModuleLogger::print("in1: %lf in2: %lf out: %lf", In1, In2, Out);
    }
    TestEndMsg();

	return true;
}
