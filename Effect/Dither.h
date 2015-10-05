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
        static const unsigned int DITHER_BITSIZE_MIN = 1;
        static const unsigned int DITHER_BITSIZE_MAX = 32;
        static const unsigned int DITHER_BITSIZE_DEFAULT = 12;

        /**
        this effect is used to dither incoming audio-samples to fit a given bitsize.
        the minimum bitsize is 1 and maximum is 32

        example:

        when setting bitsize to 4 there are only 2^4=16 possible values.
        so the value range between -1.0 and 1.0 gets divided into 16 parts
        and the output sample is the one which is nearest to the input sample.
        */
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
            unsigned char getBitsize(void) { return mBitsize; }
            void setBitsize(unsigned char Bitsize) { mBitsize = Util::ModuleHelper::clamp(Bitsize, DITHER_BITSIZE_MIN, DITHER_BITSIZE_MAX); }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_INPUT] = connection; }
            void attachBitsize(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_BITSIZE] = connection; }

        private:
            unsigned char mBitsize;
            bool bBypass;
        };
    }
}

#endif
