#include <Effect/modVCF.h>

void FxVCF::Init()
{
    setFiltertype(0);
    setFrequency(0.5);
    setResonance(0.5);
    setQ(0.5);

    out1 = out2 = out3 = out4 = 0;
    in1 = in2 = in3 = in4 = 0;
}

double FxVCF::Process(double Input)
{
    switch (lFiltertype)
    {
    case FILTER_TYPE_SIMPLE:
        return SimpleFilter(Input);
        break;

    case FILTER_TYPE_MOOG:
        return MoogFilter(Input);
        break;

    case FILTER_TYPE_MOOG2:
        return MoogFilter2(Input);
        break;

    default:
        return 0.0;
        break;
    }
}

double FxVCF::SimpleFilter(double Input)
{
    double c = pow(0.5, (128 - (dFrequency * 128)) / 16.0);
    double r = pow(0.5, ((dResonance) + 24) / 16.0);

    in1 =  (1 - r * c) * in1 - (c) * in2 + (c) * Input;
    in2 =  (1 - r * c) * in2 + (c) * in1;

    out1 = in2;
    return out1;
}

double FxVCF::MoogFilter(double Input)
{
    double f = dFrequency * 1.16;
    double fb = dResonance * (1.0 - 0.15 * f * f);

    Input -= out4 * fb;
    Input *= 0.35013 * (f*f)*(f*f);
    out1 = Input + 0.3 * in1 + (1 - f) * out1; // Pole 1
    in1  = Input;
    out2 = out1 + 0.3 * in2 + (1 - f) * out2;  // Pole 2
    in2  = out1;
    out3 = out2 + 0.3 * in3 + (1 - f) * out3;  // Pole 3
    in3  = out2;
    out4 = out3 + 0.3 * in4 + (1 - f) * out4;  // Pole 4
    in4  = out3;
    return out4;
}

double FxVCF::MoogFilter2(double Input)
{
    //Init
    double cutoff = dFrequency * 22050.0;
    double fs = 44100.0;

    double f = 2 * cutoff / fs;
    double k = 3.6 * f - 1.6 * f * f - 1;
    double p = (k + 1) * 0.5;
    double scale = exp(1 - p) * 1.386249;
    double r = dResonance * scale;
    in4 = out4;

    //Loop
    //--Inverted feed back for corner peaking
    double x = Input - r * in4;

    //Four cascaded onepole filters (bilinear transform)
    in1= x * p + out1 * p - k * in1;
    in2= in1 * p + out2 * p - k * in2;
    in3= in2 * p + out3* p - k * in3;
    in4= in3 * p + out4 * p - k* in4;

    //Clipper band limited sigmoid
    in4 = in4 - (pow(in4, 3)) / 6;

    out1 = x;
    out2 = in1;
    out3 = in2;
    out4 = in3;

    return out4;
}
