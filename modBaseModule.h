/*
 * modBaseModule.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODBASEMODULE_H_
#define MODBASEMODULE_H_

#ifndef COMPANY_NAME
#define COMPANY_NAME "SyS AudioResearch"
#endif

#ifndef PI
#define PI 3.1415926535
#endif

#ifndef MIDISCALER
#define MIDISCALER 1.0 / 127.0
#endif

#ifndef EULER
#define EULER 2.71828
#endif

#ifndef EULER_DIV_2
#define EULER_DIV_2 1.95716
#endif

#ifndef LOWEST
#define LOWEST 0.0001
#endif

#define PLUGIN_TYPE_SYNTH   1
#define PLUGIN_TYPE_EFFECT  2

#define DEBUG_MSG_MAX       1024

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

using namespace std;

namespace eLibV2
{
    class BaseModule
    {
    public:
        virtual void Init(void) = 0;
        virtual void Test(void) = 0;
        virtual std::string getModuleName(void) {return ModuleName;}

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

        void TestBeginMsg(void) {TestMode = true; dbgOutput("Begin Test");}
        void TestEndMsg(void) {TestMode = false; dbgOutput("End Test");}

        void dbgOutputF(char *format, ...)
        {
            char MsgBuffer[DEBUG_MSG_MAX];
            va_list vl;

            va_start(vl, format);
            vsprintf(MsgBuffer, format, vl);
            dbgOutput(MsgBuffer);
            va_end(vl);
        }

        void dbgOutput(char *msg)
        {
            std::string dbgString;

            dbgString.assign(ModuleName);
            dbgString.append(": ");
            dbgString.append(msg);

        #if defined(WIN32) && !defined(_CONSOLE)
            OutputDebugString(dbgString.c_str());
        #else
            cout << dbgString << endl;
        #endif
        }

    protected:
        string ModuleName;
        double dSamplerate;
        bool TestMode;
    };
}

#endif /* MODBASEMODULE_H_ */
