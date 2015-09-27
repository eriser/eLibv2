#ifndef MODBUTTERWORTHBSF_H_
#define MODBUTTERWORTHBSF_H_

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
        class ButterworthBSF : public Base::BaseEffect
        {
        public:
            ButterworthBSF(std::string name = "ButterworthBSF") :
                Base::BaseName(name)
            {
                Init();
            }

            virtual ~ButterworthBSF(void)
            {
                if (mInternalBiquad)
                    delete mInternalBiquad;
                mInternalBiquad = NULL;
            }

            void Init()
            {
                mInternalBiquad = new BiQuad();
                mCutoff = 22050.0;
                mBW = 0.5;
                calcCoefficients();
            }

            void setCutoff(const double cutoff)
            {
                mCutoff = cutoff;
                calcCoefficients();
            }

            void setBW(const double bw)
            {
                mBW = bw;
                calcCoefficients();
            }

            virtual void setSamplerate(double samplerate)
            {
                BaseModule::setSamplerate(samplerate);
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double argtan = ModuleHelper::clamp(((PI * mCutoff * mBW) / mSamplerate), -PI_DIV_2, PI_DIV_2);
                double C = tan(argtan);
                double D = 2.0 * cos((2.0 * PI * mCutoff) / mSamplerate);

                double a0 = 1.0 / (1.0 + C);
                double a1 = -a0 * D;
                double a2 = a0;
                double b1 = -a0 * D;
                double b2 = a0 * (1.0 - C);

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
            double mBW;
        };
    }
}

#endif
