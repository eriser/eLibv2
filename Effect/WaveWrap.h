#ifndef MODWAVEWRAP_H_
#define MODWAVEWRAP_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

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
            FxWaveWrap(std::string name = "FxWaveWrap") :
                Base::BaseName(name)
            {
                Init();
            }
            virtual ~FxWaveWrap() { if (triangle) delete[] triangle; }

        public:
            /* inherited */
            virtual void Init(void);
            virtual void Reset(void) {}
            virtual double Process(const double Input);
            virtual double processConnection();

        public:
            double getWrapLevel(void) { return dWrapLevel; }
            void setWrapLevel(double WrapLevel) { dWrapLevel = Util::ModuleHelper::clamp(WrapLevel, WAVEWRAP_LEVEL_MIN, WAVEWRAP_LEVEL_MAX); }

        public:
            virtual void attachInput(Base::BaseConnection *controller) { connect(WAVEWRAP_INPUT, controller); }
            virtual void attachLevel(Base::BaseConnection *controller) { connect(WAVEWRAP_LEVEL, controller); }

        private:
            double dWrapLevel;
            double *triangle;
        };
    }
}

#endif
