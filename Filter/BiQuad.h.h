#ifndef MODBIQUAD_H_
#define MODBIQUAD_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

#define FLT_MIN_PLUS    1.175494351e-38 /* min positive value */
#define FLT_MIN_MINUS  -1.175494351e-38 /* min negative value */

namespace eLibV2
{
    namespace Effect
    {
        /**
        Implements a single Bi-Quad Structure
        */
        class BiQuad : public Base::BaseName
        {
        public:
            BiQuad(void) {}
            virtual ~BiQuad(void) {}

            virtual void Init() {}
            virtual void Reset()
            {
                // flush Delays
                mXz_1 = mXz_2 = mYz_1 = mYz_2 = 0;
            }

            // Do the filter: given input xn, calculate output yn and return it
            virtual double Process(const double input)
            {
                // just do the difference equation: y(n) = a0x(n) + a1x(n-1) + a2x(n-2) - b1y(n-1) - b2y(n-2)
                double yn = (mA0 * input) + (mA1 * mXz_1) + (mA2 * mXz_2) - (mB1 * mYz_1) - (mB2 * mYz_2);
                yn = Util::ModuleHelper::clamp(yn, -1.0, 1.0);

                // underflow check
                if (
                    (yn > 0.0) && (yn < FLT_MIN_PLUS) ||
                    (yn < 0.0) && (yn > FLT_MIN_MINUS)
                    )
                    yn = 0;

                // shuffle delays
                // Y delays
                mYz_2 = mYz_1;
                mYz_1 = yn;

                // X delays
                mXz_2 = mXz_1;
                mXz_1 = input;

                return yn;
            }

            void setCoefficients(const double a0, const double a1, const double a2, const double b1, const double b2)
            {
                mA0 = a0;
                mA1 = a1;
                mA2 = a2;
                mB1 = b1;
                mB2 = b2;
            }

        private:
            // delay elements
            double mXz_1, mXz_2, mYz_1, mYz_2;

            // coefficients
            double mA0, mA1, mA2, mB1, mB2;
        };
    }
}

#endif
