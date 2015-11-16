#include <Data/PluginProgram.h>

using namespace eLibV2::Data;

void PluginProgram::addParameter(const float value)
{
    if ((UInt16)mParameterValues.size() < mNumParameters)
        mParameterValues.push_back(value);
}

void PluginProgram::setParameter(const UInt16 parameterIndex, const float value)
{
    if ((UInt16)mParameterValues.size() > parameterIndex)
        mParameterValues[parameterIndex] = value;
}

float PluginProgram::getParameter(const UInt16 parameterIndex) const
{
    if ((UInt16)mParameterValues.size() > parameterIndex)
        return mParameterValues[parameterIndex];
    else
        return 0.0f;
}

