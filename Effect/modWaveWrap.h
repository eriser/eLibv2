#ifndef MODWAVEWRAP_H_
#define MODWAVEWRAP_H_

#include <Base/modBaseEffect.h>
#include <Util/modHelper.h>

namespace eLibV2
{
    namespace Effect
    {
        static const double WAVEWRAP_LEVEL_MIN = 1.0;
        static const double WAVEWRAP_LEVEL_MAX = 10.0;

        class FxWaveWrap : public Base::BaseEffect
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
                : Base::BaseName(name) {
                Init();
            }
            ~FxWaveWrap() { if (triangle) delete[] triangle; }

            virtual void Init(void);
            virtual void Reset(void) {}
            virtual double Process(double Input);
            virtual double processConnection();

        public:
            double getWrapLevel(void) { return dWrapLevel; }
            void setWrapLevel(double WrapLevel) { dWrapLevel = ModuleHelper::clamp(WrapLevel, WAVEWRAP_LEVEL_MIN, WAVEWRAP_LEVEL_MAX); }

        public:
            void attachInput(Base::BaseConnection *controller) { connect(WAVEWRAP_INPUT, controller); }
            void attachLevel(Base::BaseConnection *controller) { connect(WAVEWRAP_LEVEL, controller); }


        private:
            double dWrapLevel;
            double *triangle;
        };
    }
}

#endif
