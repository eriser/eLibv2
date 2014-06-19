/*
 * modToneGenerator.h
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#ifndef MODTONEGENERATOR_H_
#define MODTONEGENERATOR_H_

#include "modBaseModule.h"

namespace eLibV2
{
    class NoiseGenerator : public BaseModule
    {
    public:
        NoiseGenerator();
        ~NoiseGenerator();

        void Init(void);
        void Reset(void);
        void Test(void) {}
        double Process(void);

    protected:
        double *pBuffer;
    };
}

#endif /* MODTONEGENERATOR_H_ */
