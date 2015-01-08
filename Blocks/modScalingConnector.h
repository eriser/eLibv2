#ifndef MODSCALINGCONNECTOR_H_
#define MODSCALINGCONNECTOR_H_

#include <Base/modBaseConnection.h>

namespace eLibV2
{
	class ScalingConnector : public BaseConnection
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
		virtual double processConnection()
		{
			double input = 0.0;

			for (connectionIterator it = connections.begin(); it != connections.end(); it++)
			{
				double value = it->second->processConnection();
				if (it->first == SCALING_FACTOR)
					mFactor = value;
				else if (it->first == SCALING_OFFSET)
					mOffset = value;
				else
					input += value;
			}

			ModuleLogger::print("%s::processIOs value: %lf %lf %lf -> %lf", getModuleName().c_str(), input, mFactor, mOffset, input * mFactor + mOffset);
			return input * mFactor + mOffset;
		}

		virtual void attachInput(BaseConnection *controller) { connect(SCALING_INPUT + numInputs, controller); numInputs++; }
		virtual void attachFactor(BaseConnection *controller) { connect(SCALING_FACTOR, controller); }
		virtual void attachOffset(BaseConnection *controller) { connect(SCALING_OFFSET, controller); }

	protected:
		int numInputs;
		double mFactor, mOffset;
	};
}

#endif
