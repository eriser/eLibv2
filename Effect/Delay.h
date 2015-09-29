#ifndef MODDELAY_H_
#define MODDELAY_H_

#include <Base/BaseEffect.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        class Delay : public Base::BaseEffect
        {
        public:
            enum
            {
                DELAY_BUFFER_SIZE = 22050,
                DELAY_CONNECTION_INPUT = 0,
                DELAY_CONNECTION_INDEX,
                DELAY_CONNECTION_NUM
            };

        public:
            Delay(std::string name = "Delay") :
                Base::BaseName(name),
                BaseConnection(DELAY_CONNECTION_NUM)
            {
                buffer = new double[DELAY_BUFFER_SIZE];
                memset(buffer, 0, sizeof(buffer) * DELAY_BUFFER_SIZE);
                Init();
            }
            virtual ~Delay() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection();

        public:
            void attachInput(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_INPUT] = connection; }

        private:
            void adjustIndices();

        private:
            double *buffer;
            double mixLevel;
            bool bBypass;
            unsigned int readIndex, writeIndex;
        };
    }
}

#endif
