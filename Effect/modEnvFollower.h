#ifndef MODENVFOLLOWER_H_
#define MODENVFOLLOWER_H_

#include <Base/modBaseEffect.h>
#include <Util/modHelper.h>

namespace eLibV2
{
    namespace Effect
    {
        class FxEnvFollower : public BaseEffect
        {
        public:
            FxEnvFollower(std::string name = "FxEnvFollower")
                : BaseName(name) {
                Init();
            }

            void Init(void);
            void Reset(void);
            double Process(double Input);

            double getAttack(void) { return dAttack; }
            double getRelease(void) { return dRelease; }

            void setAttack(double Attack) { dAttack = Attack; }
            void setRelease(double Release) { dRelease = Release; }

        private:
            double dAttack;
            double dRelease;
            double dLastInput;
            double dSampleRate;
            long tAttack;
            long tRelease;
        };
    }
}

#endif
