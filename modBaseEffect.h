/*
 * modBaseEffect.h
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#ifndef MODBASEEFFECT_H_
#define MODBASEEFFECT_H_

#include "modBaseModule.h"

namespace eLibV2
{
    class BaseEffect : public BaseModule
    {
    public:
        virtual void Init(void) = 0;
        virtual void Reset(void) = 0;
        virtual void Test(void) = 0;
        virtual double Process(double Input) = 0;

    protected:
        double *pBuffer;
    };
}

#endif /* MODBASEEFFECT_H_ */
