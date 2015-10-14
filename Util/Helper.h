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
            static double LinearInterpolation(const double value1, const double value2, const double fractional)
            {
                double res = value1;

                // use weighted sum method of interpolating
                res = fractional * value2 + (1.0 - fractional) * value1;

                return res;
            }

            // performs lagrange interpolation with two value-arrays and a fractional position between them
            static double LagrangeInterpolation(const double *xInput, const double *yInput, unsigned char order, const double value)
            {
                double fx = 0.0;
                double l = 1.0;
                for (unsigned char i = 0; i < order; i++)
                {
                    l = 1.0;
                    for (unsigned char j = 0; j < order; j++)
                    {
                        if (j != i)
                            l *= (value - xInput[j]) / (xInput[i] - xInput[j]);
                    }
                    fx += l * yInput[i];
                }
                return (fx);
            }

            // helper functions for conversion between time / frequency / quarter and samples
            static double ms2samples(const double ms, const double samplerate) { return (ms / 1000.0) * samplerate; }
            static double samples2ms(const double samples, const double samplerate) { return (samples / samplerate) * 1000.0; }
            static double freq2samples(const double freq, const double samplerate) { return (samplerate / freq); }
            static double samples2freq(const double samples, const double samplerate) { return (samplerate / samples); }
            static double quarter2samples(const double quarter, const double bpm, const double samplerate) { return (quarter * 60 * samplerate) / bpm; }

            // helper functions to convert from linear to logarithmic and vice versa
            static double dB2value(const double dB) { return pow(10.0, dB / 20.0); }
            static double value2dB(const double value)
            {
                if (value <= 0.0)
                    return -96.0;
                else
                    return 20.0 * log10(value);
            }

            // helper function to convert a value to a bool regarding a threshold
            static bool double2bool(const double value, const double threshold) { return (value < threshold) ? false : true; }

            static double GenerateTestSignal(void) { return ((((double)rand()) / RAND_MAX) * 2) - 1.0; }
        };
    }
}

#endif
