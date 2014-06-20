/*
 * VSTBaseParameter
 */

#include <VSTBase/VSTBaseParameter.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

VSTBaseParameter::VSTBaseParameter()
{
    mParameterName = "param";
    mUnitLabel = "s";
    mType = ParameterTypeDouble;
    mMinValue = 0;
    mMaxValue = 100;
    mInitialValue = 0;
    mCanBeAutomated = false;
}

VSTBaseParameter::VSTBaseParameter(const std::string name, const std::string label, double minValue, double maxValue, double initValue)
    : mParameterName(name)
    , mUnitLabel(label)
    , mType(ParameterTypeDouble)
    , mMinValue(minValue)
    , mMaxValue(maxValue)
    , mInitialValue(initValue)
    , mCanBeAutomated(false)
{
}

VSTBaseParameter::VSTBaseParameter(const std::string name, const std::string label, ParameterType type, double minValue, double maxValue, double initValue)
    : mParameterName(name)
    , mUnitLabel(label)
    , mType(type)
    , mMinValue(minValue)
    , mMaxValue(maxValue)
    , mInitialValue(initValue)
    , mCanBeAutomated(false)
{
}

std::string VSTBaseParameter::getValue(const double in)
{
    std::stringstream res;

    double output = (in * (mMaxValue - mMinValue)) + mMinValue;

    switch (mType)
    {
        case ParameterTypeDouble:
            res << (double)output;
            break;

        case ParameterTypeInteger:
            res << (VstInt32)output;
            break;

        default:
            res << "...";
            break;
    }
    return res.str();
}
