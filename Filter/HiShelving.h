#ifndef MODHISHELVING_H_
#define MODHISHELVING_H_

#include <Filter/EnhancedBiQuad.h.h>
#include <Util/Defines.h>
#include <cmath>

namespace eLibV2
{
    namespace Effect
    {
        /**
        Implements a hi-shelving filter
        */
        class HiShelving : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_FILTER_INPUT
            };

        public:
            HiShelving(std::string name = "HiShelving") :
                Base::BaseName(name)
            {
                Init();
            }
            virtual ~HiShelving(void)
            {
                if (mInternalBiquad)
                    delete mInternalBiquad;
                mInternalBiquad = NULL;
            }

            void Init()
            {
                mInternalBiquad = new EnhancedBiQuad();
                mCutoff = 22050.0;
                mGain = 0.0;
                calcCoefficients();
            }

            void setCutoff(const double cutoff)
            {
                mCutoff = cutoff;
                calcCoefficients();
            }

            void setGain(const double gain)
            {
                mGain = gain;
                calcCoefficients();
            }

            virtual void setSamplerate(double samplerate)
            {
                BaseModule::setSamplerate(samplerate);
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double ThetaC = (2.0 * PI * mCutoff) / mSamplerate;
                ThetaC = ModuleHelper::minval(ThetaC, mMinimumThetaC);

                double Mu = pow(10, (mGain / 40.0));
                double Beta = (1.0 + Mu) / 4.0;

                double argtan = ModuleHelper::clamp((ThetaC / 2.0), -PI_DIV_2, PI_DIV_2);
                double Delta = Beta * tan(argtan);
                double Gamma = (1.0 - Delta) / (1.0 + Delta);

                double a0 = (1.0 + Delta) / 2.0;
                double a1 = (1.0 + Gamma) / -2.0;
                double a2 = 0.0;
                double b1 = -Gamma;
                double b2 = 0.0;
                double c0 = Mu - 1.0;
                double d0 = 1.0;

                if (mInternalBiquad)
                {
                    mInternalBiquad->setCoefficients(a0, a1, a2, b1, b2);
                    mInternalBiquad->setWetDryLevel(c0, d0);
                }
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
            EnhancedBiQuad *mInternalBiquad;

            double mCutoff;
            double mGain;
        };
    }
}

#endif
