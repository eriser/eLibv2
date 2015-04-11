#ifndef MODSIGNALADD_H_
#define MODSIGNALADD_H_

#include <Base/modBaseEffect.h>
#include <Util/modHelper.h>

/// @todo the process-method does not really do what it supposed to do
namespace eLibV2
{
    namespace Effect
    {
        class FxSignalAdd : public BaseEffect
        {
        public:
            FxSignalAdd(std::string name = "FxSignalAdd")
                : BaseName(name) {}

            void Init(void) {}
            void Reset(void) {}

            double Process(double Input1, double Input2);

        private:
            double Process(double Input) { return Input; }
        };
    }
}

#endif
