#ifndef MODSCALINGCONNECTOR_H_
#define MODSCALINGCONNECTOR_H_

#include <Blocks/modBaseConnector.h>

using namespace eLibV2;

namespace eLibV2
{
	class ScalingConnector : public BaseConnector
	{
	public:
		enum
		{
			SCALING_FACTOR,
			SCALING_OFFSET,
			SCALING_INPUT
		};

	public:
		ScalingConnector(double factor = 1.0, double offset = 0.0) : BaseName("ScalingConnector"), numInputs(0), mFactor(factor), mOffset(offset) {}
		ScalingConnector(std::string name, double factor = 1.0, double offset = 0.0) : BaseName(name), mFactor(factor), mOffset(offset) {}

		virtual void Init() {}
		virtual double processIOs();

		virtual void attachInput(ControlIO *controller) { attachIO(SCALING_INPUT + numInputs, controller); numInputs++; }
		virtual void attachFactor(ControlIO *controller) { attachIO(SCALING_FACTOR, controller); }
		virtual void attachOffset(ControlIO *controller) { attachIO(SCALING_OFFSET, controller); }

	protected:
		int numInputs;
		double mFactor, mOffset;
	};
}

#endif
