#ifndef MODWAVEWRAP_H_
#define MODWAVEWRAP_H_

#include <Base/modBaseEffect.h>

namespace eLibV2
{
    class FxWaveWrap : public BaseEffect
    {
    public:
		enum { WRAP_WAVESIZE = 32768 };
		enum
		{
			WAVEWRAP_INPUT,
			WAVEWRAP_LEVEL
		};

	public:
		FxWaveWrap(std::string name = "FxWaveWrap")
			: BaseName(name) { Init(); }

		virtual void Init(void);
		virtual void Reset(void) {}
		virtual bool Test();
		virtual double Process(double Input);
		virtual double processConnection();

	public:
        double getWrapLevel(void) {return dWrapLevel;}
        void setWrapLevel(double WrapLevel) {dWrapLevel = WrapLevel;}

	public:
		void attachInput(BaseConnection *controller) { connect(WAVEWRAP_INPUT, controller); }
		void attachLevel(BaseConnection *controller) { connect(WAVEWRAP_LEVEL, controller); }


    private:
        double dWrapLevel;
        double triangle[WRAP_WAVESIZE];
    };
}

#endif
