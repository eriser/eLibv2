#include <Util/FrequencyTable.h>

using namespace eLibV2::Util;

bool FrequencyTable::initialized = false;
double FrequencyTable::freqtab[FrequencyTable::kNumNotes] = { 0 };

void FrequencyTable::SetupFreqs(void)
{
    // make frequency (Hz) table
    double k = 1.059463094359;  // 12th root of 2
    double a = 6.875; // 1.71875; // a
    a *= k;     // b
    a *= k;     // bb
    a *= k;     // c, frequency of midi note 0, should be 8.1757989156 Hz

    // 128 midi notes
    for (UInt8 FreqIndex = 0; FreqIndex < kNumNotes; FreqIndex++)
    {
        freqtab[FreqIndex] = a;
        a *= k;
    }
    initialized = true;
}

double FrequencyTable::ForNote(UInt8 note)
{
    double res = 0.0;

    if (!initialized)
        SetupFreqs();
    res = freqtab[note];
    return res;
}
