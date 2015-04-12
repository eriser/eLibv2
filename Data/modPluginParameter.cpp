#include <Data/modPluginParameter.h>

using namespace eLibV2::Data;

PluginParameter::PluginParameter(const std::string name, const std::string label, ParameterType type, double minValue, double maxValue, double initValue)
    : mParameterName(name)
    , mUnitLabel(label)
    , mType(type)
    , mMinValue(minValue)
    , mMaxValue(maxValue)
    , mInitialValue(initValue)
{
}

double PluginParameter::getValue(const double in) const
{
    double output;
    output = (in * (mMaxValue - mMinValue)) + mMinValue;

    if (isBooleanType())
        return (in < 0.5) ? 0 : 1;
    else if (isIntegerType())
        return ((float)((int)output));
    else
        return (float)output;
}

std::string PluginParameter::getValueAsString(const double in) const
{
    std::stringstream res;

    double output = getValue(in);

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

double PluginParameter::getRawValue(const double in) const
{
    return (in - mMinValue) / (mMaxValue - mMinValue);
}
