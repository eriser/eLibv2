/*
 * modBaseModule.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODBASEMODULE_H_
#define MODBASEMODULE_H_

#include <Base/modTestCase.h>
#include <Base/modBaseName.h>
#include <Base/modDebug.h>

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
    class BaseModule : public TestCase, public Debug
    {
    public:
        virtual void Init(void) = 0;

        virtual double getSamplerate(void) {return dSamplerate;}
        virtual void setSamplerate(double Samplerate) {dSamplerate = Samplerate;}

    protected:
        // helper functions for truncating given value at minimum / maximum
        double minval(double value, double minvalue) {return ((value < minvalue) ? minvalue : value);}
        double maxval(double value, double maxvalue) {return ((value > maxvalue) ? maxvalue : value);}

        // helper functions for conversion between time / frequency / quarter and samples
        virtual double ms2samples(double ms, double samplerate) {return (ms / 1000.0) * samplerate;}
        virtual double samples2ms(double samples, double samplerate) {return (samples / samplerate) * 1000.0;}
        virtual double freq2samples(double freq, double samplerate) {return (samplerate / freq);}
        virtual double samples2freq(double samples, double samplerate) {return (samplerate / samples);}
        virtual double quarter2samples(double quarter, double bpm, double samplerate) {return (quarter * 60 * samplerate) / bpm;}

        virtual double GenerateTestSignal(void) {return ((((double)rand()) / RAND_MAX) * 2) - 1.0;}

    protected:
        double dSamplerate;
    };
}

#endif /* MODBASEMODULE_H_ */
