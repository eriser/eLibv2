#ifndef MODBASEMODULE_H_
#define MODBASEMODULE_H_

#include <Base/modBaseConnection.h>

#include <Util/modDefines.h>

// std headers
#include <iostream>
#include <string>
#include <cstdarg>

// vst headers
#include <audioeffectx.h>

namespace eLibV2
{
    class BaseModule : virtual public BaseConnection
    {
    public:
        BaseModule() : BaseName("BaseModule") {}

        virtual void Init(void) = 0;

        virtual double getSamplerate(void) { return samplerate; }
        virtual void setSamplerate(double Samplerate) { samplerate = Samplerate; }

    protected:
        double samplerate;
    };
}

#endif
