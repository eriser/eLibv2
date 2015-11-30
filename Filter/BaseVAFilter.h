#ifndef MODBASEVAFILTER_H_
#define MODBASEVAFILTER_H_

#include <Filter/EnhancedBiQuad.h>
#include <Base/BaseModule.h>

#include <cmath>

namespace eLibV2
{
    namespace Filter
    {
        /**
        this module provides basic methods used for signal processing in its subclasses
        */
        class BaseVAFilter : public Base::BaseModule
        {
        public:
            enum
            {
                FILTER_CONNECTION_BYPASS = 0,
                FILTER_CONNECTION_INPUT,
                FILTER_CONNECTION_CUTOFF,
                FILTER_CONNECTION_Q,
                FILTER_CONNECTION_NUM
            };

            enum eFilterType
            {
                TYPE_LPF,
                TYPE_HPF,
                TYPE_BPF,
                TYPE_BSF
            };

        public:
            BaseVAFilter(std::string name = "BaseVAFilter") :
                Base::BaseName(name),
                Connection::BaseConnection(FILTER_CONNECTION_NUM)
            {
                m_eType = TYPE_LPF;
                m_iOrder = 1;

                m_bNLPActive = false;
                m_dSaturation = 1.0;
            }

            virtual ~BaseVAFilter()
            {

            }

            virtual void Init(void) = 0;
            virtual void Reset(void) = 0;

        public:
            /* getter/setter */
            virtual bool getBypass(void) const { return m_bBypass; }
            virtual void setBypass(const bool Bypass) { m_bBypass = Bypass; }

            virtual eFilterType getType(void) { return m_eType; }
            virtual void setType(eFilterType Type)
            {
                m_eType = Type;
                calcCoefficients();
            }

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

            virtual double getSaturation() const { return m_dSaturation; }
            virtual void setSaturation(const double Saturation) { m_dSaturation = Saturation; }

            virtual bool getNLPActive() const { return m_bNLPActive; }
            virtual void setNLPActive(const bool NLPActive) { m_bNLPActive = NLPActive; }

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

                if (!m_bBypass)
                    dOutput = Process(dInput);
                else
                    dOutput = dInput;
                return dOutput;
            }

            virtual double Process(double input) = 0;

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_INPUT] = connection; }
            void attachCutoff(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_CUTOFF] = connection; }
            void attachQ(BaseConnection *connection) { inputConnections[FILTER_CONNECTION_Q] = connection; }

        protected:
            virtual void calcCoefficients(void) = 0;

        protected:
            eFilterType m_eType;    ///< type of filter (LPF, HPF,...)
            UInt8 m_iOrder;         ///< order of the filter (1, 2, 4)
            bool m_bBypass;         ///< bypass processing of filter
            double m_dCutoff;       ///< cutoff frequency
            double m_dQ;            ///< q-factor
            bool m_bNLPActive;      ///< non-linear processing
            double m_dSaturation;   ///< saturation factor
        };
    }
}

#endif
