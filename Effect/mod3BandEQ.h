#ifndef MOD3BANDEQ_H_
#define MOD3BANDEQ_H_

#include <Base/modBaseEffect.h>

using namespace eLibV2;

namespace eLibV2
{
    static const double vsa = (1.0 / 4294967295.0);   // Very small amount (Denormal Fix)

    enum
    {
        EQ_NUM_BANDS = 4,
        EQ_NUM_POLES = 4,
        EQ_BUFFER_SIZE = 3
    };

    typedef struct
    {
        double Frequency;
        double Poles[EQ_NUM_POLES];
    } EQBand;

    class Fx3BandEQ : public BaseEffect
    {
    public:

		Fx3BandEQ() : BaseName("Fx3BandEQ") { Init(); }

        void Init(void);
        void Reset(void);

        double CalcBand(VstInt16 BandIndex, double Input);
        double Process(double Input);

        void setGain(VstInt16 Index, double Gain) {dGain[Index] = Gain;}
        void setFrequency(VstInt16 Index, double Frequency);
        void setSamplerate(double Samplerate);

		double processIOs();

    private:
        EQBand Bands[EQ_NUM_BANDS - 1];
        double dGain[EQ_NUM_BANDS];
        double dSamplerate;
        double dFrequency[EQ_NUM_BANDS - 1];

        double buffer[EQ_BUFFER_SIZE];
    };
}

#endif
