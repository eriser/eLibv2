/*
 * VSTBaseProgram
 */

#include <VSTBase/VSTBaseProgram.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

void VSTBaseProgram::addParameter(const float value)
{
    if ((VstInt32)mParameterValues.size() < mNumParameters)
        mParameterValues.push_back(value);
}

void VSTBaseProgram::setParameter(const VstInt32 parameterIndex, const float value)
{
    if ((VstInt32)mParameterValues.size() > parameterIndex)
        mParameterValues[parameterIndex] = value;
}

float VSTBaseProgram::getParameter(const VstInt32 parameterIndex) const
{
    if ((VstInt32)mParameterValues.size() > parameterIndex)
        return mParameterValues[parameterIndex];
    else
        return 0.0f;
}

