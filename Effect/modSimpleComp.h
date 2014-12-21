#ifndef MODSIMPLECOMP_H_
#define MODSIMPLECOMP_H_

#include <Base/modBaseEffect.h>

#include <math.h>

using namespace eLibV2;

namespace eLibV2
{
    class FxSimpleComp : public BaseEffect
    {
    public:
		FxSimpleComp() : BaseName("FxSimpleComp") { Init(); }

        void Init(void);
        void Reset(void);

        void setThreshold(double Threshold);
        void setRatio(double Ratio);
        void setAttack(double Attack);
        void setRelease(double Release);
        void setOutput(double Output);

        double getThreshold(void) {return dThreshold;}
        double getRatio(void) {return dRatio;}
        double getAttack(void) {return dAttack;}
        double getRelease(void) {return dRelease;}
        double getOutput(void) {return dOutput;}

        double Process(double Input);

    private:
        double dThreshold;
        double dAttack, dRelease, dEnvDecay;
        double dOutput;
        double dTransferA, dTransferB;
        double dEnv, dGain, dRatio;
    };
}

#endif
