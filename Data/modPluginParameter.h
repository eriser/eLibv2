#ifndef PLUGINPARAMETER_H_
#define PLUGINPARAMETER_H_

// std headers
#include <string>
#include <sstream>
#include <vector>

namespace eLibV2
{
	enum ParameterType
	{
		ParameterTypeBoolean = 1,
		ParameterTypeInteger,
		ParameterTypeDouble,
	};

	class PluginParameter
	{
	public:
		PluginParameter(const int id, const std::string name, const std::string label, ParameterType type, double minValue, double maxValue, double initValue = 0.0);

		std::string getName() const { return mParameterName; }
		std::string getLabel() const { return mUnitLabel; }
		std::string getValue(const double in) const;

		int getId() const { return mId; }
		double getMinValue() const { return mMinValue; }
		double getMaxValue() const { return mMaxValue; }

		bool isBooleanType() const { return (mType == ParameterTypeBoolean); }
		bool isIntegerType() const { return (mType == ParameterTypeInteger); }
		bool isDoubleType() const { return (mType == ParameterTypeDouble); }

	private:
		int mId;
		std::string mParameterName;
		std::string mUnitLabel;
		ParameterType mType;
		double mMinValue;
		double mMaxValue;
		double mInitialValue;
	};
}

#endif
