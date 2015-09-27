#ifndef MODLINKWITZRILEYLPF_H_
#define MODLINKWITZRILEYLPF_H_

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
        class LinkwitzRileyLPF : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_FILTER_INPUT
            };

        public:
            LinkwitzRileyLPF(std::string name = "LinkwitzRileyLPF") :
                Base::BaseName(name)
            {
                Init();
            }

            virtual ~LinkwitzRileyLPF(void)
            {
                if (mInternalBiquad)
                    delete mInternalBiquad;
                mInternalBiquad = NULL;
            }

            void Init()
            {
                inputConnection = NULL;
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
                double OmegaC = PI * mCutoff;
                double OmegaC2 = OmegaC * OmegaC;
                double ThetaC = ModuleHelper::clamp((OmegaC / mSamplerate), -PI_DIV_2, PI_DIV_2);

                double Kappa = OmegaC / tan(ThetaC);
                double Kappa2 = Kappa * Kappa;
                double Delta = Kappa2 + 2.0 * Kappa * OmegaC + OmegaC2;

                double a0 = OmegaC2 / Delta;
                double a1 = 2.0 * a0;
                double a2 = a0;
                double b1 = (2.0 * OmegaC2 - 2.0 * Kappa2) / Delta;
                double b2 = (Kappa2 - 2.0 * Kappa * OmegaC + OmegaC2) / Delta;

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
            BaseConnection *inputConnection;
        };
    }
}

#endif
