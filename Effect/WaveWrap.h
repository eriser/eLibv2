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

        class WaveWrap : public Base::BaseEffect
        {
        public:
            enum { WRAP_WAVESIZE = 32768 };
            enum
            {
                WAVEWRAP_CONNECTION_INPUT = 0,
                WAVEWRAP_CONNECTION_LEVEL,
                WAVEWRAP_CONNECTION_NUM
            };

        public:
            WaveWrap(std::string name = "WaveWrap") :
                BaseName(name),
                BaseConnection(WAVEWRAP_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~WaveWrap() { if (triangle) delete[] triangle; }

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
            virtual void attachInput(Base::BaseConnection *connection) { inputConnections[WAVEWRAP_CONNECTION_NUM] = connection; }
            virtual void attachLevel(Base::BaseConnection *connection) { inputConnections[WAVEWRAP_CONNECTION_LEVEL] = connection; }

        private:
            double dWrapLevel;
            double *triangle;
        };
    }
}

#endif
