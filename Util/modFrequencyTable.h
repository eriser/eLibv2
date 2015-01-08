#ifndef MODFREQUENCYTABLE_H_
#define MODFREQUENCYTABLE_H_

namespace eLibV2
{
	class FrequencyTable
	{
	public:
		enum { kNumNotes = 128 };

		static void SetupFreqs(void);
		static double ForNote(unsigned char note);

	private:
		FrequencyTable();
		static bool initialized;
		static double freqtab[kNumNotes];
	};
}
#endif