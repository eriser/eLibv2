#ifndef MODWAVETABLE_H_
#define MODWAVETABLE_H_

#include <Base/modBaseModule.h>
#include <Loader/modWaveLoader.h>

#include <time.h>
#include <math.h>
#include <stdlib.h>

using namespace eLibV2;

namespace eLibV2
{
    class BaseWavetable : public BaseModule
    {
	protected:
		enum { kWavesize = 65536 };

		enum
		{
			WAVEFORM_INDEX_NONE = 0,
			WAVEFORM_INDEX_MAX = 200
		};

		typedef struct
		{
			VstInt16 ChannelNum;
			VstInt32 WaveSize;
			std::string WaveName;
			double *WaveData;
		} Waveform;

    public:
		BaseWavetable(std::string name = "BaseWavetable")
			: BaseName(name) { Init(); }
        ~BaseWavetable();

        virtual void Init(void);
		virtual bool Test(void);

	private:
		bool AddWaveform(std::string Filename, std::string WaveName);
		bool AddWaveform(double *Wavedata, VstInt32 WaveSize, std::string WaveName, VstInt16 ChannelNum = 1);

#ifdef WIN32
		bool AddWaveform(HINSTANCE hInstance, VstInt32 ResourceID, std::string WaveName, VstInt16 ByteSize = 2, VstInt16 ChannelNum = 1);
		double* loadWaveform(HINSTANCE hInstance, int resID, double *data);
		long sizeWaveform(HINSTANCE hInstance, int resID);
#endif
		void DeleteWaveform(VstInt32 Index);

	public:
        long getWaveSize(VstInt16 WaveIndex);
        double adjustPhase(double phase);
        double adjustPhase(VstInt16 WaveIndex, double phase);
        double getWaveData(VstInt32 WaveIndex, double dPhase);
        bool getWaveName(VstInt32 WaveIndex, char* name);
        long getNumLoadedWaveforms(void) {return LoadedWaveforms;}

    private:
        Waveform Waveforms[WAVEFORM_INDEX_MAX];
        long LoadedWaveforms;
    };
}

#endif