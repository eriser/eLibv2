#include <Generator/BaseOsc.h>
#include <Effect/Clip.h>

using namespace eLibV2::Generator;
using namespace eLibV2::Effect;

int main(int argc, char**argv)
{
    BaseOscillator* BaseOsc = new BaseOscillator();
    Clip* clipper = new Clip();

    for (int i = 0; i < 100; i++)
    {
        double d = BaseOsc->Process(50);
        d = clipper->Process(d);
    }

    return 0;
}
