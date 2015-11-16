#ifndef MODENVFOLLOWER_H_
#define MODENVFOLLOWER_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        class EnvFollower : public Base::BaseEffect
        {
        public:
            EnvFollower(std::string name = "EnvFollower") :
                BaseName(name)
            {
                Init();
            }
            virtual ~EnvFollower() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);

            double getAttack(void) { return dAttack; }
            double getRelease(void) { return dRelease; }

            void setAttack(double Attack) { dAttack = Attack; }
            void setRelease(double Release) { dRelease = Release; }

        private:
            double dAttack;
            double dRelease;
            double dLastInput;
            double dSampleRate;
            SInt32 tAttack;
            SInt32 tRelease;
        };
    }
}

#endif
