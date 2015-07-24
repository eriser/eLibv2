#ifndef MODVCF_H_
#define MODVCF_H_

#include <Base/BaseEffect.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        class FxVCF : public Base::BaseEffect
        {
        public:
            enum FilterType
            {
                FILTER_TYPE_UNDEF = 0,
                FILTER_TYPE_SIMPLE,
                FILTER_TYPE_MOOG,
                FILTER_TYPE_MOOG2
            };

        public:
            FxVCF(std::string name = "FxVCF")
                : Base::BaseName(name) {
                Init();
            }

            void Init(void);
            void Reset(void) {}

            FilterType getFiltertype(void) { return eFilterType; }
            double getFrequency(void) { return dFrequency; }
            double getResonance(void) { return dResonance; }
            double getQ(void) { return dQ; }

            void setFiltertype(FilterType Type) { eFilterType = Type; }
            void setFrequency(double Frequency) { dFrequency = Frequency; }
            void setResonance(double Resonance) { dResonance = Resonance; }
            void setQ(double Q) { dQ = Q; }

            double Process(double Input);
            double MoogFilter(double Input);
            double MoogFilter2(double Input);
            double SimpleFilter(double Input);

        private:
            FilterType eFilterType;
            double dFrequency;
            double dResonance;
            double dQ;
            double out1, out2, out3, out4;
            double in1, in2, in3, in4;
        };
    }
}

#endif