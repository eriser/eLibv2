#ifndef MODSELECTORBLOCK_H_
#define MODSELECTORBLOCK_H_

#include <Effect/BaseEffect.h>

namespace eLibV2
{
    namespace Effect
    {
        class SelectorBlock : public BaseEffect
        {
        public:
            enum
            {
                CONNECTION_SELECTOR_MAXINPUT = 16
            };

        public:
            SelectorBlock(std::string name = "SelectorBlock") :
                Base::BaseName(name),
                BaseConnection(CONNECTION_SELECTOR_MAXINPUT, 1)
            {
                Init();
            }
            virtual ~SelectorBlock() {}

            /* inherited */
            virtual void Init(void)
            {
                uiInputSwitch = 0;
            }

            virtual void Reset(void);

            void setInputSwitch(const UInt16 InputSwitch) { uiInputSwitch = InputSwitch; }
            UInt16 getInputSwitch(void) { return uiInputSwitch; }

            virtual double processConnection(void);

        public:
            void attachInput(const UInt16 InputIndex, BaseConnection *connection)
            {
                if (InputIndex < CONNECTION_SELECTOR_MAXINPUT)
                    inputConnections[InputIndex] = connection;
            }

        private:
            virtual double Process(const double input) { return 0.0; }

        private:
            UInt16 uiInputSwitch;
        };
    }
}

#endif
