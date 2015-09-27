#ifndef MODBUTTERWORTHHPF_H_
#define MODBUTTERWORTHHPF_H_

#include <Filter/BiQuad.h.h>
#include <Util/Defines.h>
#include <cmath>

namespace eLibV2
{
    namespace Effect
    {
        /**
        Implements a single Bi-Quad Structure
        */
        class ButterworthHPF : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_FILTER_INPUT
            };

        public:
            ButterworthHPF(std::string name = "ButterworthHPF") :
                Base::BaseName(name)
            {
                Init();
            }

            virtual ~ButterworthHPF(void)
            {
                if (mInternalBiquad)
                    delete mInternalBiquad;
                mInternalBiquad = NULL;
            }

            void Init()
            {
                mInternalBiquad = new BiQuad();
                mCutoff = 22050.0;
                calcCoefficients();
            }

            void setCutoff(const double cutoff)
            {
                mCutoff = cutoff;
                calcCoefficients();
            }

            virtual void setSamplerate(double samplerate)
            {
                BaseModule::setSamplerate(samplerate);
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double argtan = ModuleHelper::clamp(((PI * mCutoff) / mSamplerate), -PI_DIV_2, PI_DIV_2);
                double C = tan(argtan);
                double C2 = C * C;

                double a0 = 1.0 / (1.0 + SQRT_2 * C + C2);
                double a1 = -2.0 * a0;
                double a2 = a0;
                double b1 = 2.0 * a0 * (C2 - 1.0);
                double b2 = a0 * (1.0 - SQRT_2 * C + C2);

                if (mInternalBiquad)
                    mInternalBiquad->setCoefficients(a0, a1, a2, b1, b2);
            }

            void Reset()
            {
                if (mInternalBiquad)
                    mInternalBiquad->Reset();
            }

            virtual double processConnection()
            {
                double input = 0.0;

                if (inputConnections[0] != NULL)
                    input = inputConnections[0]->processConnection();
                // ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

                return Process(input);
            }

            // Do the filter: given input xn, calculate output yn and return it
            double Process(double input)
            {
                double output = 0.0;

                if (mInternalBiquad)
                    output = mInternalBiquad->Process(input);
                return output;
            }

        public:
            void attachInput(BaseConnection *connection) { inputConnections[0] = connection; }

        private:
            BiQuad *mInternalBiquad;

            double mCutoff;
        };
    }
}

#endif
