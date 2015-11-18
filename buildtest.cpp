#include <Generator/BaseOsc.h>
#include <Loader/WaveLoader.h>
#include <Loader/LPSLoader.h>
#include <Effect/Clip.h>
#include <Util/Logger.h>

using namespace eLibV2::Generator;
using namespace eLibV2::Loader;
using namespace eLibV2::Effect;
using namespace eLibV2::Util;

int main(int argc, char**argv)
{
    BaseOscillator* BaseOsc = new BaseOscillator();
    WaveLoader* loader = new WaveLoader();
    LPSLoader* lps = new LPSLoader();
    Clip* clipper = new Clip();
    
    clipper->setClipMode(Clip::CLIP_MODE_BOTH);
    clipper->setClipLevel(0.5);

    loader->Load("./TestData/testwave_32bit_stereo.wav");
    
    for (int i = 0; i < 100; i++)
    {
        double d = BaseOsc->Process(50);
        d = clipper->Process(d);
    }
    
    loader->Unload();
    
    delete lps;
    delete loader;
    delete clipper;
    delete BaseOsc;
    
    return 0;
}
