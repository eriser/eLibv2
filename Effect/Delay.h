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
                DELAY_BUFFER_SIZE = 44100,
                DELAY_CONNECTION_INPUT = 0,
                DELAY_CONNECTION_LENGTH,
                DELAY_CONNECTION_NUM
            };

        public:
            Delay(std::string name = "Delay") :
                BaseName(name),
                BaseConnection(DELAY_CONNECTION_NUM)
            {
                buffer = new double[DELAY_BUFFER_SIZE];
                memset(buffer, 0, sizeof(buffer) * DELAY_BUFFER_SIZE);
                mixLevel = 0.5;
                delayLength = 1000;

                Init();
            }
            virtual ~Delay() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection();

        public:
            void setDelayLength(unsigned int length);

            void attachDelayLength(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_LENGTH] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_INPUT] = connection; }

        private:
            void adjustIndices();

        private:
            double *buffer;
            double mixLevel;
            unsigned int delayLength;
            bool bBypass;
            int readIndex, writeIndex;
        };
    }
}

#endif
