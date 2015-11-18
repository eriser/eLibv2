#ifndef MODWAVETABLE_H_
#define MODWAVETABLE_H_

#include <Base/BaseModule.h>
#include <Loader/WaveLoader.h>
#include <Util/Helper.h>

#include <ctime>
#include <cmath>
#include <cstdlib>

#include <string>
#include <vector>

namespace eLibV2
{
    namespace Generator
    {
        class BaseWavetable : public Base::BaseName
        {
        public:
            enum
            {
                WAVETABLE_WAVEFORM_NONE = 0,
                WAVETABLE_WAVEFORM_SINE,
                WAVETABLE_WAVEFORM_ENIS,
                WAVETABLE_WAVEFORM_TRIANGLE_BL,
                WAVETABLE_WAVEFORM_SAWUP_BL,
                WAVETABLE_WAVEFORM_SAWDN_BL,
                WAVETABLE_WAVEFORM_PULSE_BL,
                WAVETABLE_WAVEFORM_TRIANGLE,
                WAVETABLE_WAVEFORM_SAWUP,
                WAVETABLE_WAVEFORM_SAWDN,
                WAVETABLE_WAVEFORM_PULSE,
                WAVETABLE_WAVEFORM_NOISE,
                WAVETABLE_WAVEFORM_SH
            };

        private:
            enum
            {
                WAVETABLE_SAMPLE_BANDLIMIT = 63, // needs to be odd value
                WAVETABLE_SAMPLE_SIZE = 0x8000   // size is limited to a UInt16
            };

            typedef struct
            {
                UInt16 ChannelNum;
                UInt16 WaveSize;
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
            bool AddWaveform(const double *Wavedata, const UInt16 WaveSize, const std::string WaveName, const UInt8 ChannelNum = 1);

#if defined(WIN32)
            bool AddWaveform(const HINSTANCE hInstance, const UInt16 ResourceID, const std::string WaveName, const UInt8 ByteSize = 2, const UInt8 ChannelNum = 1);
            double* loadWaveform(const HINSTANCE hInstance, const UInt16 resID, double *data, const UInt8 ByteSize = 2, const UInt8 ChannelNum = 1);
#endif
            void DeleteWaveform(const UInt16 Index);

        public:
            SInt32 getWaveSize(const UInt16 WaveIndex);
            double adjustPhase(const UInt16 WaveIndex, const double phase);
            double getWaveData(const UInt16 WaveIndex, const double dPhase);
            bool getWaveName(const UInt16 WaveIndex, char* name);
            SInt32 getNumLoadedWaveforms(void) { return Waveforms.size(); }

        private:
            std::vector<Waveform> Waveforms;

            static BaseWavetable *instance;
        };
    }
}

#endif