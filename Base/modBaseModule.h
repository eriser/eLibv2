#ifndef MODBASEMODULE_H_
#define MODBASEMODULE_H_

#include <Blocks/modControlIO.h>

#include <Util/modDefines.h>
#include <Util/modHelper.h>
#include <Util/modTestCase.h>

// std headers
#include <iostream>
#include <string>
#include <cstdarg>

// vst headers
#include <audioeffectx.h>

namespace eLibV2
{
	class BaseModule : public ControlIO, public ModuleHelper, public ModuleTestCase
    {
    public:
		BaseModule() : BaseName("BaseModule") {}

        virtual void Init(void) = 0;

        virtual double getSamplerate(void) {return dSamplerate;}
        virtual void setSamplerate(double Samplerate) {dSamplerate = Samplerate;}

		virtual bool Test() { return true; }

		// for testing purposes
		virtual void TestBeginMsg() { ModuleLogger::print("Begin Test (%s))", getModuleName().c_str()); }
		virtual void TestEndMsg() { ModuleLogger::print("End Test (%s))", getModuleName().c_str()); }

	protected:
        double dSamplerate;
    };
}

#endif
