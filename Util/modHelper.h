#ifndef MODHELPER_H_
#define MODHELPER_H_

#include <string>

namespace eLibV2
{
	class ModuleHelper
	{
	protected:
		// helper functions for truncating given value at minimum / maximum
		double minval(double value, double minvalue) { return ((value < minvalue) ? minvalue : value); }
		double maxval(double value, double maxvalue) { return ((value > maxvalue) ? maxvalue : value); }
		double clamp(double value, double minvalue, double maxvalue) { double res = minval(value, minvalue); res = maxval(value, maxvalue); return res; }

		// helper functions for conversion between time / frequency / quarter and samples
		virtual double ms2samples(double ms, double samplerate) { return (ms / 1000.0) * samplerate; }
		virtual double samples2ms(double samples, double samplerate) { return (samples / samplerate) * 1000.0; }
		virtual double freq2samples(double freq, double samplerate) { return (samplerate / freq); }
		virtual double samples2freq(double samples, double samplerate) { return (samplerate / samples); }
		virtual double quarter2samples(double quarter, double bpm, double samplerate) { return (quarter * 60 * samplerate) / bpm; }

		virtual double GenerateTestSignal(void) { return ((((double)rand()) / RAND_MAX) * 2) - 1.0; }
	};
}

#endif
