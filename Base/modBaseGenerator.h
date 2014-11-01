/*
* modBaseGenerator.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODBASEGENERATOR_H_
#define MODBASEGENERATOR_H_

#include <Base/modBaseGenerator.h>
#include <Generator/modWavetable.h>

namespace eLibV2
{
	class BaseGenerator : public BaseModule
	{
	public:
		enum
		{
			kNumFrequencies = 128,  // 128 midi notes
		};

	public:
		virtual void Init(void) = 0;
		virtual void Reset(void) = 0;
		virtual double Process(VstInt16 Note) = 0;

	protected:
		void SetupFreqs(void)
		{
			// make frequency (Hz) table
			double k = 1.059463094359;  // 12th root of 2
			double a = 1.71875; // a
			a *= k;     // b
			a *= k;     // bb
			a *= k;     // c, frequency of midi note 0

			// 128 midi notes
			for (VstInt16 FreqIndex = 0; FreqIndex < kNumFrequencies; FreqIndex++)
			{
				freqtab[FreqIndex] = a;
				a *= k;
			}
		}

		double freqtab[kNumFrequencies];
		BaseWavetable *pBaseWavetable;
	};
}

#endif
