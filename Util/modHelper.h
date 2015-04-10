#ifndef MODHELPER_H_
#define MODHELPER_H_

#include <string>

namespace eLibV2
{
    class ModuleHelper
    {
    public:
        // helper functions for truncating given value at minimum / maximum
        static double minval(double value, double minvalue) { return ((value < minvalue) ? minvalue : value); }
        static double maxval(double value, double maxvalue) { return ((value > maxvalue) ? maxvalue : value); }
        static double clamp(double value, double minvalue, double maxvalue) { double res = minval(value, minvalue); res = maxval(value, maxvalue); return res; }

        // helper functions for conversion between time / frequency / quarter and samples
        static double ms2samples(double ms, double samplerate) { return (ms / 1000.0) * samplerate; }
        static double samples2ms(double samples, double samplerate) { return (samples / samplerate) * 1000.0; }
        static double freq2samples(double freq, double samplerate) { return (samplerate / freq); }
        static double samples2freq(double samples, double samplerate) { return (samplerate / samples); }
        static double quarter2samples(double quarter, double bpm, double samplerate) { return (quarter * 60 * samplerate) / bpm; }

        static double GenerateTestSignal(void) { return ((((double)rand()) / RAND_MAX) * 2) - 1.0; }
    };
}

#endif
