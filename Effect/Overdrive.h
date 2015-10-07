#ifndef MODOVERDRIVE_H_
#define MODOVERDRIVE_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        static const double OVERDRIVE_LEVEL_MIN = 0.0;
        static const double OVERDRIVE_LEVEL_MAX = 100.0;

        class Overdrive : public Base::BaseEffect
        {
        public:
            enum
            {
                OVERDRIVE_CONNECTION_INPUT = 0,
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
            virtual void Init() { setLevel(50.0); }
            virtual void Reset() {}
            virtual double Process(const double input);
            virtual double processConnection();

        public:
            void setLevel(double Level) { dLevel = Util::ModuleHelper::clamp(Level, OVERDRIVE_LEVEL_MIN, OVERDRIVE_LEVEL_MAX); }
            double getLevel(void) { return dLevel; }

        public:
            void attachInput(BaseConnection *connection) { inputConnections[OVERDRIVE_CONNECTION_INPUT] = connection; }
            void attachLevel(BaseConnection *connection) { inputConnections[OVERDRIVE_CONNECTION_LEVEL] = connection; }

        protected:
            double dLevel;
        };
    }
}

#endif
