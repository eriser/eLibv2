#ifndef MODWAVESHAPER_H_
#define MODWAVESHAPER_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        class WaveShaper : public Base::BaseEffect
        {
        public:
            enum
            {
                WAVESHAPER_CONNECTION_BYPASS = 0,
                WAVESHAPER_CONNECTION_INPUT,
                WAVESHAPER_CONNECTION_NUM
            };

        public:
            WaveShaper(std::string name = "WaveShaper") :
                BaseName(name),
                BaseConnection(WAVESHAPER_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~WaveShaper() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void) {}
            virtual double Process(const double Input);
            virtual double processConnection(void);

        public:
            /* getter/setter */
            void setPositiveExponent(const double Exponent) { m_dPositiveExponent = Exponent; }
            double getPositiveExponent(void) { return m_dPositiveExponent; }

            void setNegativeExponent(const double Exponent) { m_dNegativeExponent = Exponent; }
            double getNegativeExponent(void) { return m_dNegativeExponent; }

            void setNumStages(const unsigned char NumStages) { m_iNumStages = NumStages; }
            unsigned char getNumStages(void) { return m_iNumStages; }

            void setInvertStages(const bool InvertStages) { m_bInvertStages = InvertStages; }
            bool getInvertStages(void) { return m_bInvertStages; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[WAVESHAPER_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[WAVESHAPER_CONNECTION_INPUT] = connection; }

        private:
            double m_dPositiveExponent, m_dNegativeExponent;
            unsigned char m_iNumStages;
            bool m_bInvertStages;
        };
    }
}

#endif
