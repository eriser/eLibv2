#ifndef MODRESAMPLE_H_
#define MODRESAMPLE_H_

#include <Base/BaseEffect.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        static const double RESAMPLE_RATE_MIN = 20.0;
        static const double RESAMPLE_RATE_MAX = 44100.0;

        /**
        this effect is used to resample incoming audio-samples to fit a given samplerate.

        a simple sample&hold-algorithm is used
        */
        class Resample : public Base::BaseEffect
        {
        public:
            enum
            {
                RESAMPLE_CONNECTION_BYPASS = 0,
                RESAMPLE_CONNECTION_INPUT,
                RESAMPLE_CONNECTION_RATE,
                RESAMPLE_CONNECTION_NUM
            };

        public:
            Resample(std::string name = "Resample") :
                Base::BaseName(name),
                BaseConnection(RESAMPLE_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~Resample() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection();
            virtual void setSamplerate(const double Samplerate);

        public:
            /* getter/setter */
            double getRate(void) { return m_dRate; }
            void setRate(const double Rate);

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[RESAMPLE_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[RESAMPLE_CONNECTION_INPUT] = connection; }
            void attachRate(BaseConnection *connection) { inputConnections[RESAMPLE_CONNECTION_RATE] = connection; }

        private:
            void adjustSamplePeriod()
            {
                m_dSamplePeriod = mSamplerate / m_dRate;
            }

        private:
            bool m_bBypass;
            double m_dRate;
            double m_dCurrentValue;
            double m_dCurrentSampleIndex;
            double m_dSamplePeriod;
        };
    }
}

#endif
