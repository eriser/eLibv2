#ifndef MODDITHER_H_
#define MODDITHER_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        static const UInt16 DITHER_BITSIZE_MIN = 1;
        static const UInt16 DITHER_BITSIZE_MAX = 32;
        static const UInt16 DITHER_BITSIZE_DEFAULT = 12;

        /**
        this effect is used to dither incoming audio-samples to fit a given bitsize.
        the minimum bitsize is 1 and maximum is 32

        example:

        when setting bitsize to 4 there are only 2^4=16 possible values.
        so the value range between -1.0 and 1.0 gets divided into 16 parts
        and the output sample is the one which is nearest to the input sample.
        */
        class Dither : public Base::BaseEffect
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
            Dither(std::string name = "Dither") :
                BaseName(name),
                BaseConnection(DITHER_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~Dither() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection(void);

        public:
            /* getter/setter */
            UInt8 getBitsize(void) { return m_uiBitsize; }
            void setBitsize(const UInt8 Bitsize) { m_uiBitsize = Util::ModuleHelper::clamp(Bitsize, DITHER_BITSIZE_MIN, DITHER_BITSIZE_MAX); }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_INPUT] = connection; }
            void attachBitsize(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_BITSIZE] = connection; }

        private:
            UInt8 m_uiBitsize;
        };
    }
}

#endif
