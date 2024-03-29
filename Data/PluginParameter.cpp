#include <Data/PluginParameter.h>

using namespace eLibV2::Data;

PluginParameter::PluginParameter(const std::string id, const std::string label, ParameterType type, double minValue, double maxValue, double initValue)
    : mParameterId(id)
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
        return ((float)((SInt16)output));
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
        if (in < ((mMaxValue - mMinValue) / 2))
            res << "false";
        else
            res << "true";
        break;

    case ParameterTypeDouble:
        res << (double)output;
        break;

    case ParameterTypeInteger:
        res << (SInt16)output;
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
