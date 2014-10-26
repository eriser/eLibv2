/*
 * modVCF.h
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#ifndef MODVCF_H_
#define MODVCF_H_

#include <Base/modBaseEffect.h>

namespace eLibV2
{
    enum
    {
        FILTER_TYPE_SIMPLE = 0,
        FILTER_TYPE_MOOG,
        FILTER_TYPE_MOOG2
    };

    class FxVCF : public BaseEffect
    {
    public:
        FxVCF();
        ~FxVCF() {}

        void Init(void);
        void Reset(void) {}

        long getFiltertype(void) {return lFiltertype;}
        double getFrequency(void) {return dFrequency;}
        double getResonance(void) {return dResonance;}
        double getQ(void) {return dQ;}

        void setFiltertype(long FilterType) {lFiltertype = FilterType;}
        void setFrequency(double Frequency) {dFrequency = Frequency;}
        void setResonance(double Resonance) {dResonance = Resonance;}
        void setQ(double Q) {dQ = Q;}

        double Process(double Input);
        double MoogFilter(double Input);
        double MoogFilter2(double Input);
        double SimpleFilter(double Input);

    private:
        long lFiltertype;
        double dFrequency;
        double dResonance;
        double dQ;
        double out1, out2, out3, out4;
        double in1, in2, in3, in4;
    };
}

#endif /* MODVCF_H_ */
