#include <Data/modPluginProgram.h>

using namespace eLibV2::Data;

void PluginProgram::addParameter(const float value)
{
    if ((int)mParameterValues.size() < mNumParameters)
        mParameterValues.push_back(value);
}

void PluginProgram::setParameter(const unsigned long parameterIndex, const float value)
{
    if ((unsigned long)mParameterValues.size() > parameterIndex)
        mParameterValues[parameterIndex] = value;
}

float PluginProgram::getParameter(const unsigned long parameterIndex) const
{
    if ((unsigned long)mParameterValues.size() > parameterIndex)
        return mParameterValues[parameterIndex];
    else
        return 0.0f;
}

