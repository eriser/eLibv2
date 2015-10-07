#ifndef MODEQ3BAND_H_
#define MODEQ3BAND_H_

#include <Base/BaseEffect.h>

#include <math.h>
#include <memory.h>

namespace eLibV2
{
    namespace Effect
    {
        class EQ3Band : public Base::BaseEffect
        {
        private:
            enum
            {
                EQ_NUM_BANDS = 4,
                EQ_NUM_POLES = 4,
                EQ_BUFFER_SIZE = 3
            };

            typedef struct
            {
                double Frequency;
                double Poles[EQ_NUM_POLES];
            } EQBand;

        public:
            EQ3Band(std::string name = "EQ3Band") :
                BaseName(name),
                BaseConnection(1, 1)
            {
                Init();
            }
            virtual ~EQ3Band() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);

            virtual double processConnection();

            /* setter */
            void setGain(VstInt16 Index, double Gain) { dGain[Index] = Gain; }
            void setFrequency(VstInt16 Index, double Frequency);
            void setSamplerate(double Samplerate);

        private:
            double CalcBand(VstInt16 BandIndex, double Input);

        private:
            EQBand Bands[EQ_NUM_BANDS - 1];
            double dGain[EQ_NUM_BANDS];
            double dFrequency[EQ_NUM_BANDS - 1];

            double buffer[EQ_BUFFER_SIZE];
        };
    }
}

#endif
