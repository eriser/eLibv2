#ifndef VSTBASEPROGRAM_H_
#define VSTBASEPROGRAM_H_

// std headers
#include <vector>
#include <map>
#include <string>

// vst headers
#include <audioeffectx.h>

namespace eLibV2
{
    namespace VSTBase
    {
        class VSTBaseProgram
        {
        public:
            VSTBaseProgram(const VstInt32 numParameters) { mNumParameters = numParameters; }
            ~VSTBaseProgram() {}

            void addParameter(const float value);
            void setParameter(const VstInt32 parameterIndex, const float value);
            float getParameter(const VstInt32 parameterIndex) const;

            void setName(const std::string name) { mProgramName = name; }
            std::string getName() const { return mProgramName; }

        private:
            VstInt32 mNumParameters;
            std::vector<float> mParameterValues;
            std::string mProgramName;
        };

        typedef std::vector<VSTBaseProgram> VSTBasePrograms;
    }
}

#endif /* VSTBASEPROGRAM_H_ */