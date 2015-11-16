#ifndef MODFREQUENCYTABLE_H_
#define MODFREQUENCYTABLE_H_

#include <Util/Types.h>

namespace eLibV2
{
    namespace Util
    {
        class FrequencyTable
        {
        public:
            enum { kNumNotes = 128 };

            static void SetupFreqs(void);
            static double ForNote(UInt8 note);

        private:
            FrequencyTable();
            static bool initialized;
            static double freqtab[kNumNotes];
        };
    }
}
#endif