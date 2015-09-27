#ifndef MODDITHER_H_
#define MODDITHER_H_

#include <Base/BaseEffect.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        static const VstInt16 DITHER_BITSIZE_MIN = 1;
        static const VstInt16 DITHER_BITSIZE_MAX = 32;

        class FxDither : public Base::BaseEffect
        {
        public:
            enum
            {
                DITHER_CONNECTION_BYPASS = 0,
                DITHER_CONNECTION_INPUT,
                DITHER_CONNECTION_BITSIZE,
                DITHER_CONNECTION_NUM
            };

        public:
            FxDither(std::string name = "FxDither") :
                Base::BaseName(name),
                BaseConnection(DITHER_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~FxDither() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection();

        public:
            bool getBypass(void) { return bBypass; }
            void setBypass(bool Bypass) { bBypass = Bypass; }
            VstInt16 getBitsize(void) { return mBitsize; }
            void setBitsize(VstInt16 Bitsize) { mBitsize = Util::ModuleHelper::clamp(Bitsize, DITHER_BITSIZE_MIN, DITHER_BITSIZE_MAX); }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_INPUT] = connection; }
            void attachBitsize(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_BITSIZE] = connection; }

        private:
            VstInt16 mBitsize;
            bool bBypass;
        };
    }
}

#endif
