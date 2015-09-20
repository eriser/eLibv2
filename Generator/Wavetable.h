#ifndef MODWAVETABLE_H_
#define MODWAVETABLE_H_

#include <Base/BaseModule.h>
#include <Loader/WaveLoader.h>
#include <Util/Helper.h>

#include <time.h>
#include <math.h>
#include <stdlib.h>

#include <string>
#include <vector>

namespace eLibV2
{
    namespace Generator
    {
        class BaseWavetable : public Base::BaseModule
        {
        protected:
            enum
            {
                WAVETABLE_SAMPLE_BANDLIMIT = 63, // needs to be odd value
                WAVETABLE_SAMPLE_SIZE = 65536
            };

            typedef struct
            {
                VstInt16 ChannelNum;
                VstInt32 WaveSize;
                std::string WaveName;
                double *WaveData;
            } Waveform;

        private:
            BaseWavetable(std::string name = "BaseWavetable") :
                Base::BaseName(name)
            {
                Init();
            }

        public:
            static BaseWavetable *getInstance();
            ~BaseWavetable();

            virtual void Init(void);

        private:
            bool AddWaveform(const std::string Filename, const std::string WaveName);
            bool AddWaveform(const double *Wavedata, const unsigned int WaveSize, const std::string WaveName, const unsigned char ChannelNum = 1);

#if defined(WIN32)
            bool AddWaveform(const HINSTANCE hInstance, const unsigned int ResourceID, const std::string WaveName, const unsigned char ByteSize = 2, const unsigned char ChannelNum = 1);
            double* loadWaveform(const HINSTANCE hInstance, const unsigned int resID, double *data, const unsigned char ByteSize = 2, const unsigned char ChannelNum = 1);
#endif
            void DeleteWaveform(const unsigned int Index);

        public:
            long getWaveSize(const unsigned int WaveIndex);
            double adjustPhase(const unsigned int WaveIndex, const double phase);
            double getWaveData(const unsigned int WaveIndex, const double dPhase);
            bool getWaveName(const unsigned int WaveIndex, char* name);
            long getNumLoadedWaveforms(void) { return Waveforms.size(); }

        private:
            std::vector<Waveform> Waveforms;

            static BaseWavetable *instance;
        };
    }
}

#endif