/*
 * modBaseLFO.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODBASELFO_H_
#define MODBASELFO_H_

#include "modBaseOsc.h"

namespace eLibV2
{
    class BaseLFO : public BaseOscillator
    {
    public:
        void Init(void);
        double Process(void);
    };
}

#endif /* MODBASELFO_H_ */
