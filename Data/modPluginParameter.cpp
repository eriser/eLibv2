#include <Data/modPluginParameter.h>

using namespace eLibV2;

PluginParameter::PluginParameter(const int id, const std::string name, const std::string label, ParameterType type, double minValue, double maxValue, double initValue)
	: mId(id)
	, mParameterName(name)
	, mUnitLabel(label)
	, mType(type)
	, mMinValue(minValue)
	, mMaxValue(maxValue)
	, mInitialValue(initValue)
{
}

std::string PluginParameter::getValue(const double in) const
{
	std::stringstream res;

	double output = (in * (mMaxValue - mMinValue)) + mMinValue;

	switch (mType)
	{
	case ParameterTypeBoolean:
		res << (in < ((mMaxValue - mMinValue) / 2)) ? "false" : "true";
		break;

	case ParameterTypeDouble:
		res << (double)output;
		break;

	case ParameterTypeInteger:
		res << (int)output;
		break;

	default:
		res << "...";
		break;
	}
	return res.str();
}
