#ifndef MODSIGNALADD_H_
#define MODSIGNALADD_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        /// @todo the process-method does not really do what it supposed to do
        class FxSignalAdd : public Base::BaseName
        {
        public:
            FxSignalAdd(std::string name = "FxSignalAdd") :
                Base::BaseName(name)
            {
                Init();
            }
            virtual ~FxSignalAdd() {}

            /* inherited */
            virtual void Init(void) {}
            virtual void Reset(void) {}

            double Process2(const double Input1, const double Input2);

        private:
            virtual double Process(const double Input) { return Input; }
        };
    }
}

#endif
