#ifndef MODDITHER_H_
#define MODDITHER_H_

#include <Base/BaseEffect.h>
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
            virtual double processConnection();

        public:
            /* getter/setter */
            unsigned char getBitsize(void) { return m_uiBitsize; }
            void setBitsize(const unsigned char Bitsize) { m_uiBitsize = Util::ModuleHelper::clamp(Bitsize, DITHER_BITSIZE_MIN, DITHER_BITSIZE_MAX); }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_INPUT] = connection; }
            void attachBitsize(BaseConnection *connection) { inputConnections[DITHER_CONNECTION_BITSIZE] = connection; }

        private:
            unsigned char m_uiBitsize;
            bool m_bBypass;
        };
    }
}

#endif
