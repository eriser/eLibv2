#ifndef MODSWITCHABLEAPF_H_
#define MODSWITCHABLEAPF_H_

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
        class SwitchableAPF : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_FILTER_INPUT
            };

        public:
            SwitchableAPF(std::string name = "SwitchableAPF") :
                BaseName(name)
            {
                Init();
            }

            virtual ~SwitchableAPF(void)
            {
                if (m_pInternalBiquad)
                    delete m_pInternalBiquad;
                m_pInternalBiquad = NULL;
            }

            void Init()
            {
                m_pInternalBiquad = new BiQuad();
                m_dCutoff = 22050.0;
                m_dQ = 0.707;
                m_bSecondOrder = true;
            }

            double getCutoff() { return m_dCutoff; }
            void setCutoff(const double Cutoff)
            {
                m_dCutoff = Cutoff;
                calcCoefficients();
            }

            double getQ() { return m_dQ; }
            double setQ(const double Q)
            {
                m_dQ = Q;
                calcCoefficients();
            }

            virtual void setSamplerate(const double Samplerate)
            {
                BaseModule::setSamplerate(Samplerate);
                calcCoefficients();
            }

            bool getSecondOrder() { return m_bSecondOrder; }
            void setSecondOrder(const bool SecondOrder)
            {
                m_bSecondOrder = SecondOrder;
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0;

                if (m_bSecondOrder)
                {
                    double argtan = ModuleHelper::clamp(((PI * m_dQ) / mSamplerate), -PI_DIV_2, PI_DIV_2);
                    double Alpha = (tan(argtan) - 1.0) / (tan(argtan) + 1.0);
                    double ThetaC = 2.0 * PI * m_dCutoff / mSamplerate;
                    double Beta = -cos(ThetaC);

                    a0 = -Alpha;
                    a1 = Beta * (1.0 - Alpha);
                    a2 = 1.0;
                    b1 = Beta * (1.0 - Alpha);
                    b2 = -Alpha;
                }
                else
                {
                    double argtan = ModuleHelper::clamp(((PI * m_dCutoff) / mSamplerate), -PI_DIV_2, PI_DIV_2);
                    double Alpha = (tan(argtan) - 1.0) / (tan(argtan) + 1.0);

                    a0 = Alpha;
                    a1 = 1.0;
                    a2 = 0.0;
                    b1 = Alpha;
                    b2 = 0.0;
                }

                if (m_pInternalBiquad)
                    m_pInternalBiquad->setCoefficients(a0, a1, a2, b1, b2);
            }

            void Reset()
            {
                if (m_pInternalBiquad)
                    m_pInternalBiquad->Reset();
            }

            virtual double processConnection()
            {
                double dInput = 0.0;

                if (inputConnections[0] != NULL)
                    dInput = inputConnections[0]->processConnection();
                // ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

                return Process(dInput);
            }

            // Do the filter: given input xn, calculate output yn and return it
            double Process(double input)
            {
                double dOutput = 0.0;

                if (m_pInternalBiquad)
                    dOutput = m_pInternalBiquad->Process(input);
                return dOutput;
            }

        public:
            void attachInput(BaseConnection *connection) { inputConnections[0] = connection; }

        private:
            BiQuad *m_pInternalBiquad;
            bool m_bSecondOrder;
            double m_dCutoff, m_dQ;
        };
    }
}

#endif
