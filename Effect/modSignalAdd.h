/*
 * modSignalAdd.h
 *
 *  Created on: 13.01.2012
 *      Author: dedokter
 */

#ifndef MODSIGNALADD_H_
#define MODSIGNALADD_H_

#include <Base/modBaseEffect.h>

namespace eLibV2
{
    class FxSignalAdd : public BaseEffect
    {
    public:
        FxSignalAdd();
        ~FxSignalAdd() {}

        void Init(void);
        void Reset(void);
        void Test(void);

        double Process(double Input) {return Input;}
        double Process(double Input1, double Input2);
    };
}

#endif /* MODSIGNALADD_H_ */
