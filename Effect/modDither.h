#ifndef MODDITHER_H_
#define MODDITHER_H_

#include <Base/modBaseEffect.h>
#include <Util/modLogger.h>
#include <Util/modHelper.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        static const VstInt16 DITHER_BITSIZE_MIN = 1;
        static const VstInt16 DITHER_BITSIZE_MAX = 32;

        class FxDither : public BaseEffect
        {
        public:
            enum
            {
                CONNECTION_DITHER_INPUT,
                CONNECTION_DITHER_BITSIZE
            };

        public:
            FxDither(std::string name = "FxDither") : BaseName(name) { Init(); }

            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(double Input);
            virtual double processConnection();

        public:
            VstInt16 getBitsize(void) { return mBitsize; }
            void setBitsize(VstInt16 Bitsize) { mBitsize = ModuleHelper::clamp(Bitsize, DITHER_BITSIZE_MIN, DITHER_BITSIZE_MAX); }

        public:
            void attachInput(BaseConnection *controller) { connect(CONNECTION_DITHER_INPUT, controller); }
            void attachBitsize(BaseConnection *controller) { connect(CONNECTION_DITHER_BITSIZE, controller); }

        private:
            VstInt16 mBitsize;
        };
    }
}

#endif
