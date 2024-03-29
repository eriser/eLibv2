#ifndef MODCASCADEDEQCONSTANTQ_H_
#define MODCASCADEDEQCONSTANTQ_H_

#include <Filter/BaseFilter.h>
#include <Connection/InputConnection.h>
#include <Filter/ParametricConstantQ.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a hi-shelving filter
        */
        class CascadedEQConstantQ : public BaseFilter
        {
        public:
            CascadedEQConstantQ(std::string name = "CascadedEQConstantQ") :
                BaseName(name),
                BaseConnection(FILTER_CONNECTION_NUM),
                BaseFilter()
            {
                m_uiNumberOfStages = 10;

                Init();
            }

            CascadedEQConstantQ(UInt8 NumberOfStages, std::string name = "CascadedEQConstantQ") :
                BaseName(name)
            {
                m_uiNumberOfStages = ModuleHelper::clamp(NumberOfStages, 1, 31);

                Init();
            }

            virtual void calcCoefficients() {}

            virtual ~CascadedEQConstantQ(void)
            {
                if (m_pStages)
                {
                    for (UInt8 stageIndex = 0; stageIndex < m_uiNumberOfStages; ++stageIndex)
                        delete m_pStages[stageIndex];
                    delete[] m_pStages;
                }
                m_pStages = NULL;

                if (m_pInput)
                    delete m_pInput;
                m_pInput = NULL;
            }

            void Init(void)
            {
                double N = 10.0 / m_uiNumberOfStages;
                double Q = sqrt(pow(2.0, N)) / (pow(2.0, N) - 1.0);
                double q = m_uiNumberOfStages / 10.0;
                double Cutoff = 0.0;
                m_dGain = 0.0;

                m_pInput = new Connection::InputConnection("eq.input");
                m_pStages = new ParametricConstantQ*[m_uiNumberOfStages];
                for (UInt8 stageIndex = 0; stageIndex < m_uiNumberOfStages; ++stageIndex)
                {
                    m_pStages[stageIndex] = new ParametricConstantQ();
                    if (stageIndex == 0)
                        m_pStages[stageIndex]->attachInput(m_pInput);
                    else
                        m_pStages[stageIndex]->attachInput(m_pStages[stageIndex - 1]);

                    Cutoff = (1000.0 * pow(2.0, (stageIndex - (m_uiNumberOfStages / 2)) / q));
                    m_pStages[stageIndex]->setCutoff(Cutoff);
                    m_pStages[stageIndex]->setQ(Q);
                    m_pStages[stageIndex]->setGain(m_dGain);
                }
            }

            void setGain(const double Gain)
            {
                m_dGain = Gain;
                if (m_pStages)
                {
                    for (UInt8 stageIndex = 0; stageIndex < m_uiNumberOfStages; ++stageIndex)
                        m_pStages[stageIndex]->setGain(Gain);
                }
            }

            virtual void setSamplerate(const double Samplerate)
            {
                BaseModule::setSamplerate(Samplerate);
                if (m_pStages)
                {
                    for (UInt8 stageIndex = 0; stageIndex < m_uiNumberOfStages; ++stageIndex)
                        m_pStages[stageIndex]->setSamplerate(Samplerate);
                }
            }

            void Reset(void)
            {
                if (m_pStages)
                {
                    for (UInt8 stageIndex = 0; stageIndex < m_uiNumberOfStages; ++stageIndex)
                        m_pStages[stageIndex]->Reset();
                }
            }

            virtual double processConnection(void)
            {
                double input = 0.0;

                return Process(input);
            }

            double Process(double input)
            {
                double dOutput = 0.0;

                if (m_pStages)
                    dOutput = m_pStages[m_uiNumberOfStages - 1]->processConnection();
                return dOutput;
            }

        public:
            void attachInput(BaseConnection *connection) { inputConnections[0] = connection; m_pStages[0]->attachInput(connection); }

        private:
            Connection::InputConnection *m_pInput;
            ParametricConstantQ **m_pStages;
            UInt8 m_uiNumberOfStages;
            double m_dQ, m_dGain;
        };
    }
}

#endif
