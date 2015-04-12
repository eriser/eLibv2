#include <Generator/modNoiseGenerator.h>

using namespace eLibV2::Generator;

double NoiseGenerator::Process(void)
{
    return Util::ModuleHelper::GenerateTestSignal();
}
