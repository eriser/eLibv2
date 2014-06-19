/*
 * VSTBaseProgram
 */

#include <VSTBase/VSTBaseProgram.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

VSTBaseProgram::VSTBaseProgram()
{
}

void VSTBaseProgram::setParameter(const VstInt32 parameterIndex, const float value)
{
    mParameters[parameterIndex] = value;
}

float VSTBaseProgram::getParameter(const VstInt32 parameterIndex) const
{
    if (mParameters.count(parameterIndex))
        return mParameters.at(parameterIndex);
    else
        return 0.0f;
}

