/*
 * modBaseModule.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODBASEMODULE_H_
#define MODBASEMODULE_H_

#include <Blocks/modControlIO.h>

#include <Util/modDebug.h>
#include <Util/modHelper.h>

// std headers
#include <iostream>
#include <string>
#include <cstdarg>

// vst headers
#include <audioeffectx.h>

// platform headers
#ifdef WIN32
#include <windows.h>
#endif

namespace eLibV2
{
	class BaseModule : public ModuleDebug, public ModuleHelper, public ControlIO
    {
    public:
        virtual void Init(void) = 0;

        virtual double getSamplerate(void) {return dSamplerate;}
        virtual void setSamplerate(double Samplerate) {dSamplerate = Samplerate;}

    protected:
        double dSamplerate;
    };
}

#endif /* MODBASEMODULE_H_ */
