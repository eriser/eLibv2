/*
 * modWavetable.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODWAVETABLE_H_
#define MODWAVETABLE_H_

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "Base/modBaseModule.h"

namespace eLibV2
{
    enum
    {
        WAVEFORM_INDEX_NONE = 0,
        WAVEFORM_INDEX_MAX = 200
    };

    enum
    {
        kWavesize = 65536      // samples (must be power of 2 here)
    };

    typedef struct
    {
        VstInt16 ChannelNum;
        VstInt32 WaveSize;
        std::string WaveName;
        double *WaveData;
    } Waveform;

    class BaseWavetable : public BaseModule
    {
    public:
        BaseWavetable();
        ~BaseWavetable();

        void Init(void);
        void Reset(void);
        void Test(void) {}

        bool AddWaveform(string Filename, string WaveName);
        bool AddWaveform(double *Wavedata, VstInt32 WaveSize, string WaveName, VstInt16 ChannelNum = 1);
#if defined(WIN32) && !defined(_CONSOLE)
        bool AddWaveform(HINSTANCE hInstance, VstInt32 ResourceID, string WaveName, VstInt16 ByteSize = 2, VstInt16 ChannelNum = 1);
#endif

        long getWaveSize(VstInt16 WaveIndex);
        double adjustPhase(double phase);
        double adjustPhase(VstInt16 WaveIndex, double phase);
        double getWaveData(VstInt32 WaveIndex, double dPhase);
        bool getWaveName(VstInt32 WaveIndex, char* name);
        long getNumLoadedWaveforms(void) {return LoadedWaveforms;}

    private:
        Waveform Waveforms[WAVEFORM_INDEX_MAX];
        long LoadedWaveforms;

#ifdef WIN32
        double* loadWaveform(HINSTANCE hInstance, int resID, double *data);
        long sizeWaveform(HINSTANCE hInstance, int resID);
#endif
        void DeleteWaveform(VstInt32 Index);
    };
}

#endif /* MODWAVETABLE_H_ */
