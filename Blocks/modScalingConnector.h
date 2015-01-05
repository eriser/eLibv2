#ifndef MODSCALINGCONNECTOR_H_
#define MODSCALINGCONNECTOR_H_

#include <Blocks/modBaseConnector.h>

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
		ScalingConnector(std::string name = "ScalingConnector", double factor = 1.0, double offset = 0.0)
			: BaseName(name), numInputs(0), mFactor(factor), mOffset(offset) {}

		virtual void Init() {}
		virtual double processConnection();

		virtual void attachInput(BaseConnector *controller) { connect(SCALING_INPUT + numInputs, controller); numInputs++; }
		virtual void attachFactor(BaseConnector *controller) { connect(SCALING_FACTOR, controller); }
		virtual void attachOffset(BaseConnector *controller) { connect(SCALING_OFFSET, controller); }

	protected:
		int numInputs;
		double mFactor, mOffset;
	};
}

#endif
