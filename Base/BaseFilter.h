#ifndef MODBASEFILTER_H_
#define MODBASEFILTER_H_

#include <Filter/EnhancedBiQuad.h>
#include <Base/BaseModule.h>

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides basic methods used for signal processing in its subclasses
        */
        class BaseFilter : public BaseModule
        {
        public:
            enum
            {
                FILTER_CONNECTION_BYPASS = 0,
                FILTER_CONNECTION_INPUT,
                FILTER_CONNECTION_CUTOFF,
                FILTER_CONNECTION_Q,
                FILTER_CONNECTION_BW,
                FILTER_CONNECTION_GAIN,
                FILTER_CONNECTION_NUM
            };

        public:
            BaseFilter(std::string name = "BaseFilter") :
                BaseName(name),
                BaseConnection(FILTER_CONNECTION_NUM)
            {
                m_pInternalBiquad = new Filter::EnhancedBiQuad();
                mMinimumThetaC = 0.0005;
            }
            virtual ~BaseFilter()
            {
                if (m_pInternalBiquad)
                    delete m_pInternalBiquad;
                m_pInternalBiquad = NULL;
            }

            virtual void Init(void) = 0;

            /**
            reset internal BiQuad-module
            */
            virtual void Reset(void)
            {
                if (m_pInternalBiquad)
                    m_pInternalBiquad->Reset();
            }

        public:
            /* getter/setter */
            virtual bool getBypass(void) const { return m_bBypass; }
            virtual void setBypass(const bool Bypass) { m_bBypass = Bypass; }

            virtual UInt8 getOrder(void) const { return m_iOrder; }
            virtual void setOrder(const UInt8 Order)
            {
                m_iOrder = Order;
                calcCoefficients();
            }

            virtual double getCutoff() const { return m_dCutoff; }
            virtual void setCutoff(const double Cutoff)
            {
                m_dCutoff = Cutoff;
                calcCoefficients();
            }

            virtual double getQ() const { return m_dQ; }
            virtual void setQ(const double Q)
            {
                m_dQ = Q;
                calcCoefficients();
            }

            virtual double getBW() const { return m_dBW; }
            virtual void setBW(const double BW)
            {
                m_dBW = BW;
                calcCoefficients();
            }

            virtual double getGain() const { return m_dGain; }
            virtual void setGain(const double Gain)
            {
                m_dGain = Gain;
                calcCoefficients();
            }

            virtual void setSamplerate(const double Samplerate)
            {
                BaseModule::setSamplerate(Samplerate);
                calcCoefficients();
            }

            virtual double processConnection(void)
            {
                double dInput = 0.0, dOutput = 0.0;

                if (isInputConnected(FILTER_CONNECTION_BYPASS))
                    setBypass(ModuleHelper::double2bool(inputConnections[FILTER_CONNECTION_BYPASS]->processConnection(), 0.5));
                if (isInputConnected(FILTER_CONNECTION_INPUT))
                    dInput = inputConnections[FILTER_CONNECTION_INPUT]->processConnection();
                if (isInputConnected(FILTER_CONNECTION_CUTOFF))
                    setCutoff(inputConnections[FILTER_CONNECTION_CUTOFF]->processConnection());
                if (isInputConnected(FILTER_CONNECTION_Q))
                    setQ(inputConnections[FILTER_CONNECTION_Q]->processConnection());
                if (isInputConnected(FILTER_CONNECTION_BW))
                    setBW(inputConnections[FILTER_CONNECTION_BW]->processConnection());

                if (!m_bBypass)
                    dOutput = Process(dInput);
                else
                    dOutput = dInput;
                return dOutput;
            }

            // Do the filter: given input xn, calculate output yn and return it
            virtual double Process(double input)
            {
                double dOutput = 0.0;

                if (m_pInternalBiquad)
                    dOutput = m_pInternalBiquad->Process(input);
                return dOutput;
            }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_INPUT] = connection; }
            void attachCutoff(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_CUTOFF] = connection; }
            void attachQ(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_Q] = connection; }
            void attachBW(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_BW] = connection; }
            void attachGain(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_GAIN] = connection; }

        protected:
            virtual void calcCoefficients(void) = 0;

        protected:
            Filter::EnhancedBiQuad *m_pInternalBiquad;
            UInt8 m_iOrder;         ///< order of the filter
            bool m_bBypass;         ///< bypass processing of filter
            double m_dCutoff;       ///< cutoff frequency
            double m_dQ;            ///< q-factor
            double m_dBW;           ///< filter-bandwidth
            double m_dGain;         ///< filter-gain
            double mMinimumThetaC;  ///< minimum value of theta_c used in filter-modules
        };
    }
}

#endif
