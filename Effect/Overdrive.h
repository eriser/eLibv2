#ifndef MODOVERDRIVE_H_
#define MODOVERDRIVE_H_

#include <Effect/BaseEffect.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        static const double OVERDRIVE_LEVEL_MIN = 0.0;
        static const double OVERDRIVE_LEVEL_MAX = 100.0;

        class Overdrive : public BaseEffect
        {
        public:
            enum
            {
                OVERDRIVE_CONNECTION_BYPASS = 0,
                OVERDRIVE_CONNECTION_INPUT,
                OVERDRIVE_CONNECTION_LEVEL,
                OVERDRIVE_CONNECTION_NUM
            };

        public:
            Overdrive(std::string name = "Overdrive") :
                BaseName(name),
                BaseConnection(OVERDRIVE_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~Overdrive() {}

            /* inherited */
            virtual void Init(void) { setLevel(50.0); }
            virtual void Reset(void) {}
            virtual double Process(const double input);
            virtual double processConnection(void);

        public:
            void setLevel(double Level) { dLevel = Util::ModuleHelper::clamp(Level, OVERDRIVE_LEVEL_MIN, OVERDRIVE_LEVEL_MAX); }
            double getLevel(void) { return dLevel; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[OVERDRIVE_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[OVERDRIVE_CONNECTION_INPUT] = connection; }
            void attachLevel(BaseConnection *connection) { inputConnections[OVERDRIVE_CONNECTION_LEVEL] = connection; }

        protected:
            double dLevel;
        };
    }
}

#endif
