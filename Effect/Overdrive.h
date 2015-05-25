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

        class FxOverdrive : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_OVERDRIVE_INPUT,
                CONNECTION_OVERDRIVE_LEVEL
            };

        public:
            FxOverdrive(std::string name = "FxOverdrive") : Base::BaseName(name) { Init(); }

            void Init() { setLevel(50.0); }
            void Reset() {}
            double Process(double input);
            virtual double processConnection();

        public:
            void setLevel(double Level) { dLevel = Util::ModuleHelper::clamp(Level, OVERDRIVE_LEVEL_MIN, OVERDRIVE_LEVEL_MAX); }
            double getLevel(void) { return dLevel; }

        public:
            void attachInput(BaseConnection *controller) { connect(CONNECTION_OVERDRIVE_INPUT, controller); }
            void attachLevel(BaseConnection *controller) { connect(CONNECTION_OVERDRIVE_LEVEL, controller); }

        protected:
            double dLevel;
        };
    }
}

#endif
