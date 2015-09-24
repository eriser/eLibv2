#ifndef MODENHANCEDBIQUAD_H_
#define MODENHANCEDBIQUAD_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        /**
        Implements an enhanced Bi-Quad Structure with dry/wet input
        */
        class EnhancedBiQuad : public Base::BaseEffect
        {
        public:
            EnhancedBiQuad(void)
            {
                Init();
            }
            virtual ~EnhancedBiQuad(void) {}

            virtual void Init()
            {
                // initialize wet-level to 1.0 which means operation like standard bi-quad
                setWetDryLevel(1.0, 0.0);
            }

            virtual void Reset()
            {
                // flush Delays
                mXz_1 = mXz_2 = mYz_1 = mYz_2 = 0;
            }

            // Do the filter: given input xn, calculate output yn and return it
            virtual double Process(const double input)
            {
                // just do the difference equation: feedback = a0x(n) + a1x(n-1) + a2x(n-2) - b1y(n-1) - b2y(n-2)
                // y(n) = c0 * feedback + d0x(n)
                double feedback = (mA0 * input) + (mA1 * mXz_1) + (mA2 * mXz_2) - (mB1 * mYz_1) - (mB2 * mYz_2);

                feedback = Util::ModuleHelper::clamp(feedback, -1.0, 1.0);

                // underflow check
                if (
                    (feedback > 0.0) && (feedback < FLT_MIN_PLUS) ||
                    (feedback < 0.0) && (feedback > FLT_MIN_MINUS)
                    )
                    feedback = 0.0;

                // shuffle delays
                // Y delays
                mYz_2 = mYz_1;
                mYz_1 = feedback;

                // X delays
                mXz_2 = mXz_1;
                mXz_1 = input;

                return (mC0 * feedback) + (mD0 * input);
            }

            void setCoefficients(const double a0, const double a1, const double a2, const double b1, const double b2)
            {
                mA0 = a0;
                mA1 = a1;
                mA2 = a2;
                mB1 = b1;
                mB2 = b2;
            }

            void setWetDryLevel(const double wetLevel, const double dryLevel)
            {
                mC0 = Util::ModuleHelper::clamp(wetLevel, 0.0, 1.0);
                mD0 = Util::ModuleHelper::clamp(dryLevel, 0.0, 1.0);
            }

        private:
            // delay elements
            double mXz_1, mXz_2, mYz_1, mYz_2;

            // coefficients
            double mA0, mA1, mA2, mB1, mB2, mC0, mD0;
        };
    }
}

#endif
