#ifndef MODBASEOSC_H_
#define MODBASEOSC_H_

#include <Generator/BaseGenerator.h>
#include <Generator/Wavetable.h>

#include <Util/FrequencyTable.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Generator
    {
        static const double BASEOSC_COARSE_RANGE = 24.0;
        static const double BASEOSC_FINE_RANGE = 100.0;
        static const double QUADPHASE_DIVIDER = 8.0;

        /**
        this class implements a basic oscillator module using a wavetable
        */
        class BaseOscillator : public Generator::BaseGenerator
        {
        protected:
            enum
            {
                OSC_CONNECTION_NOTE = 0,
                OSC_CONNECTION_WAVEFORM,
                OSC_CONNECTION_COARSE,
                OSC_CONNECTION_FINETUNE,
                OSC_CONNECTION_NUM
            };

        public:
            BaseOscillator(std::string name = "BaseOscillator") :
                BaseName(name),
                BaseConnection(OSC_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~BaseOscillator() {}

        public:
            /* inherited methods */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const UInt8 Note);
            virtual double processConnection(void);
            virtual void setSamplerate(const double Samplerate);

            virtual void adjustPhases(UInt8 Note);

        public:
            /* set and get methods */
            void setWaveform(SInt32 Waveform);
            void setCoarse(const double Coarse);
            void setFinetune(const double Finetune);
            void setPulseWidth(const double PulseWidth);
            void adjustScaler();

            SInt32 getWaveform(void) const { return m_lWaveform; }
            double getCoarse(void) const { return m_dCoarse; }
            double getFinetune(void) const { return m_dFinetune; }
            double getPulseWidth(void) const { return m_dPulseWidth; }
            SInt32 getNumWaveforms(void);

            double getQuadOutput(void) { return m_dQuadOutput; }

        public:
            /* attach methods */
            void attachNote(Connection::BaseConnection *connection) { inputConnections[OSC_CONNECTION_NOTE] = connection; }
            void attachWaveform(Connection::BaseConnection *connection) { inputConnections[OSC_CONNECTION_WAVEFORM] = connection; }
            void attachCoarse(Connection::BaseConnection *connection) { inputConnections[OSC_CONNECTION_COARSE] = connection; }
            void attachFinetune(Connection::BaseConnection *connection) { inputConnections[OSC_CONNECTION_FINETUNE] = connection; }

        protected:
            SInt32 m_lWaveform;
            double m_dCoarse, m_dFinetune;
            double m_dPhase, m_dQuadPhase, m_dScaler;
            double m_dQuadOutput;
            double m_dPulseWidth;
        };
    }
}

#endif

