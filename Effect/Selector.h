#ifndef MODSELECTOR_H_
#define MODSELECTOR_H_

#include <Base/BaseEffect.h>

namespace eLibV2
{
    namespace Effect
    {
        class Selector : public Base::BaseEffect
        {
        public:
            enum
            {
                CONNECTION_SELECTOR_INPUT1,
                CONNECTION_SELECTOR_INPUT2,
                CONNECTION_SELECTOR_INPUT3,
                CONNECTION_SELECTOR_INPUT4,
                CONNECTION_SELECTOR_MAXINPUT
            };

        public:
            Selector(std::string name = "Selector") :
                Base::BaseName(name)
            {
                Init();
            }
            virtual ~Selector() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);

            void setInputSwitch(unsigned int InputSwitch) { uiInputSwitch = InputSwitch; }
            unsigned int getInputSwitch(void) { return uiInputSwitch; }

            virtual double processConnection();

        public:
            void attachInput(unsigned int InputIndex, BaseConnection *controller)
            {
                if (InputIndex < CONNECTION_SELECTOR_MAXINPUT)
                    connect(InputIndex, controller);
            }

        private:
            virtual double Process(double input) { return 0.0; }

        private:
            unsigned int uiInputSwitch;
        };
    }
}

#endif
