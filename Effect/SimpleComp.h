#ifndef MODSIMPLECOMP_H_
#define MODSIMPLECOMP_H_

#include <Base/BaseEffect.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        class FxSimpleComp : public Base::BaseEffect
        {
        public:
            FxSimpleComp(std::string name = "FxSimpleComp") :
                Base::BaseName(name)
            {
                Init();
            }
            virtual ~FxSimpleComp() {}

        public:
            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);

        public:
            /* getter/setter */
            void setThreshold(double Threshold);
            void setRatio(double Ratio);
            void setAttack(double Attack);
            void setRelease(double Release);
            void setOutput(double Output);

            double getThreshold(void) { return dThreshold; }
            double getRatio(void) { return dRatio; }
            double getAttack(void) { return dAttack; }
            double getRelease(void) { return dRelease; }
            double getOutput(void) { return dOutput; }

        private:
            double dThreshold;
            double dAttack, dRelease, dEnvDecay;
            double dOutput;
            double dTransferA, dTransferB;
            double dEnv, dGain, dRatio;
        };
    }
}

#endif
