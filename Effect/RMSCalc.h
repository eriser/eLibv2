#ifndef MODRMSCALCULATOR_H_
#define MODRMSCALCULATOR_H_

#include <Effect/BaseEffect.h>

namespace eLibV2
{
    namespace Effect
    {
        /**
        RMSCalculator is used to calculate the root-mean-squared of a stream of signals
        */
        class RMSCalculator : public Base::BaseName
        {
        public:
            RMSCalculator(std::string name = "RMSCalculator") :
                Base::BaseName(name)
            {
                Init();
            }
            virtual ~RMSCalculator() {}

        public:
            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);

            /**
            processing method of the effect
            @param Input this parameter is not used
            @return the smoothed rms-value of the input signals
            */
            virtual double Process(const double Input);

        public:
            /**
            input new signal to the processed input stream
            @param Input signal input
            */
            void InputSample(const double Input);

        private:
            UInt16 mSamples;
            double mRms;
            double mAlpha, mGain;
            double mRmsSmoothed;
        };
    }
}

#endif
