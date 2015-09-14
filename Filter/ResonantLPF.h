#ifndef MODRESONANTLPF_H_
#define MODRESONANTLPF_H_

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
        class ResonantLPF : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_FILTER_INPUT
            };

        public:
            ResonantLPF(void)
            {
                Init();
            }

            virtual ~ResonantLPF(void)
            {
                if (mInternalBiquad)
                    delete mInternalBiquad;
                mInternalBiquad = NULL;
            }

            void Init()
            {
                mInternalBiquad = new BiQuad();
                setCutoff(22050.0);
                setQ(10.0);
            }

            void setCutoff(const double cutoff)
            {
                mCutoff = cutoff;
                calcCoefficients();
            }

            void setQ(const double q)
            {
                mQ = q;
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

                double d = 1.0 / mQ;

                double BetaNumerator = 1.0 - ((d / 2.0) * (sin(ThetaC)));
                double BetaDenominator = 1.0 + ((d / 2.0) * (sin(ThetaC)));
                double Beta = 0.5 * (BetaNumerator / BetaDenominator);

                double Gamma = (0.5 + Beta) * (cos(ThetaC));
                double a0 = (0.5 + Beta - Gamma) / 2.0;
                double a1 = (0.5 + Beta - Gamma);
                double a2 = (0.5 + Beta - Gamma) / 2.0;
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

                if (isConnected(CONNECTION_FILTER_INPUT))
                    input = connections[CONNECTION_FILTER_INPUT]->processConnection();
                ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

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
            void attachInput(BaseConnection *controller) { connect(CONNECTION_FILTER_INPUT, controller); }

        private:
            BiQuad *mInternalBiquad;

            double mCutoff;
            double mQ;
        };
    }
}

#endif
