/*
* modInputConnector.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODINPUTCONNECTOR_H_
#define MODINPUTCONNECTOR_H_

#include <Base/modConnector.h>

#include <vector>

namespace eLibV2
{
	class InputConnector: public Connector
	{
	public:
		virtual void putValue(double value) { mValue = value; }
		virtual double processAudioInputs();
		virtual double processControlInputs();

		double processInputs() { return mValue; }

	protected:
		double mValue;
	};
}

#endif
