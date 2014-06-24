/*
 * modOverdrive.h
 *
 *  Created on: 06.01.2012
 *      Author: dedokter
 */

#ifndef MODOVERDRIVE_H_
#define MODOVERDRIVE_H_

#include "Base/modBaseModule.h"

namespace eLibV2
{
    class FxOverdrive : public BaseModule
    {
    public:
        FxOverdrive();
        ~FxOverdrive() {}

        void Init();
        void Reset();
        double Process();

        void setLevel(double Level) {dLevel = Level;}
        double getLevel(void) {return dLevel;}

    private:
        double dLevel;
    };
}

#endif /* MODOVERDRIVE_H_ */
