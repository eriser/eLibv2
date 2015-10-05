#ifndef MODHELPER_H_
#define MODHELPER_H_

#include <string>

namespace eLibV2
{
    namespace Util
    {
        class ModuleHelper
        {
        public:
            // helper functions for truncating given value at minimum / maximum
            static int minval(const int value, const int minvalue) { return ((value < minvalue) ? minvalue : value); }
            static double minval(const double value, const double minvalue) { return ((value < minvalue) ? minvalue : value); }

            static int maxval(const int value, const int maxvalue) { return ((value > maxvalue) ? maxvalue : value); }
            static double maxval(const double value, const double maxvalue) { return ((value > maxvalue) ? maxvalue : value); }

            static int clamp(const int value, const int minvalue, const int maxvalue) { int res = value; res = minval(res, minvalue); res = maxval(res, maxvalue); return res; }
            static double clamp(const double value, const double minvalue, const double maxvalue) { double res = value; res = minval(res, minvalue); res = maxval(res, maxvalue); return res; }

            static double wrap(const double value, const double range)
            {
                double res = value;
                if (res < 0.0)
                {
                    while (res < range)
                        res += range;
                }
                else
                {
                    while (res > range)
                        res -= range;
                }
                return res;
            }

            // performs linear interpolation with two values and a fractional position between them
            static double interpolate(const double value1, const double value2, const double fractional)
            {
                double res = value1;

                // use weighted sum method of interpolating
                res = fractional * value2 + (1.0 - fractional) * value1;

                return res;
            }

            // helper functions for conversion between time / frequency / quarter and samples
            static double ms2samples(double ms, double samplerate) { return (ms / 1000.0) * samplerate; }
            static double samples2ms(double samples, double samplerate) { return (samples / samplerate) * 1000.0; }
            static double freq2samples(double freq, double samplerate) { return (samplerate / freq); }
            static double samples2freq(double samples, double samplerate) { return (samplerate / samples); }
            static double quarter2samples(double quarter, double bpm, double samplerate) { return (quarter * 60 * samplerate) / bpm; }

            static double GenerateTestSignal(void) { return ((((double)rand()) / RAND_MAX) * 2) - 1.0; }
        };
    }
}

#endif
