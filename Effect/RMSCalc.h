#ifndef MODRMSCALCULATOR_H_
#define MODRMSCALCULATOR_H_

#include <Base/BaseEffect.h>

namespace eLibV2
{
    namespace Effect
    {
        enum RMSMode
        {
            RMS_MODE_UNDEF = 0
        };

        class RMSCalculator : public Base::BaseEffect
        {
        public:
            RMSCalculator(std::string name = "RMSCalculator")
                : Base::BaseName(name) {
                Init();
            }

            void Init(void);
            void Reset(void);

            void setMode(RMSMode Mode) { eMode = Mode; }
            RMSMode getMode(void) { return eMode; }

            void InputSample(const double Input);
            double Process(const double Input);

        private:
            RMSMode eMode;
            unsigned int mSamples;
            double mRms;
            double mAlpha, mGain;
            double mRmsSmoothed;
        };
    }
}

#endif
