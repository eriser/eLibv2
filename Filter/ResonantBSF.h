#ifndef MODRESONANTBSF_H_
#define MODRESONANTBSF_H_

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
        class ResonantBSF : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_FILTER_INPUT
            };

        public:
            ResonantBSF(std::string name = "ResonantBSF") :
                Base::BaseName(name)
            {
                Init();
            }

            virtual ~ResonantBSF(void)
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
                double ThetaC = (2.0 * PI * mCutoff) / mSamplerate;
                ThetaC = ModuleHelper::minval(ThetaC, mMinimumThetaC);

                double argtan = ThetaC * (mBW / 2.0);
                double BetaNumerator = 1.0 - tan(argtan);
                double BetaDenominator = 1.0 + tan(argtan);
                double Beta = 0.5 * (BetaNumerator / BetaDenominator);

                double Gamma = (0.5 + Beta) * (cos(ThetaC));
                double a0 = 0.5 + Beta;
                double a1 = -2.0 * Gamma;
                double a2 = 0.5 + Beta;
                double b1 = -2.0 * Gamma;
                double b2 = 2.0 * Beta;

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