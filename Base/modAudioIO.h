/*
* modAudioIO.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODAUDIOIO_H_
#define MODAUDIOIO_H_

#include <vector>

namespace eLibV2
{
	class AudioIO
	{
	public:
		// polymophism doesn't work without virtual!!!
		virtual void connectAudioInput(AudioIO* input) { audioInputs.push_back(input); }
		virtual double processAudioInputs();

	protected:
		std::vector<AudioIO*> audioInputs;
	};
}

#endif
