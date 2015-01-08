#include <Util/modFrequencyTable.h>

using namespace eLibV2;

bool FrequencyTable::initialized = false;
double FrequencyTable::freqtab[FrequencyTable::kNumNotes] = { 0 };

void FrequencyTable::SetupFreqs(void)
{
	// make frequency (Hz) table
	double k = 1.059463094359;  // 12th root of 2
	double a = 1.71875; // a
	a *= k;     // b
	a *= k;     // bb
	a *= k;     // c, frequency of midi note 0

	// 128 midi notes
	for (unsigned char FreqIndex = 0; FreqIndex < kNumNotes; FreqIndex++)
	{
		freqtab[FreqIndex] = a;
		a *= k;
	}
	initialized = true;
}

double FrequencyTable::ForNote(unsigned char note)
{
	double res = 0.0;

	if (!initialized)
		SetupFreqs();
	if ((note >= 0) && (note < kNumNotes))
		res = freqtab[note];
	return res;
}
