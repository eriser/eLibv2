#ifndef MODPARAMETRICCONSTANTQ_H_
#define MODPARAMETRICCONSTANTQ_H_

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
        class ParametricConstantQ : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_FILTER_INPUT
            };

        public:
            ParametricConstantQ(std::string name = "ParametricConstantQ") :
                Base::BaseName(name)
            {
                Init();
            }
            virtual ~ParametricConstantQ(void)
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
                mQ = 0.707;
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

            void setQ(const double q)
            {
                mQ = q;
                calcCoefficients();
            }

            virtual void setSamplerate(const double Samplerate)
            {
                BaseModule::setSamplerate(Samplerate);
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double argtan = ModuleHelper::clamp(((PI * mCutoff) / mSamplerate), -PI_DIV_2, PI_DIV_2);

                double K = tan(argtan);
                double K2 = K * K;
                double Mu = pow(10.0, (mGain / 20.0));
                double D0 = 1.0 + (1.0 / mQ) * K + K2;
                double E0 = 1.0 + (1.0 / (Mu * mQ)) * K + K2;

                double Alpha = 1.0 + (Mu / mQ) * K + K2;
                double Beta = 2.0 * (K2 - 1.0);
                double Gamma = 1.0 - (Mu / mQ) * K + K2;
                double Delta = 1.0 - (1.0 / mQ) * K + K2;
                double Eta = 1.0 - (1.0 / (Mu * mQ)) * K + K2;

                double a0, a1, a2, b1, b2, c0, d0;
                if (mGain > 0.0)
                {
                    a0 = Alpha / D0;
                    a1 = Beta / D0;
                    a2 = Gamma / D0;
                    b1 = Beta / D0;
                    b2 = Delta / D0;
                }
                else
                {
                    a0 = D0 / E0;
                    a1 = Beta / E0;
                    a2 = Delta / E0;
                    b1 = Beta / E0;
                    b2 = Eta / E0;
                }
                c0 = 1.0;
                d0 = 0.0;

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
            double mQ;
        };
    }
}

#endif
