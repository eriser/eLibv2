#ifndef VSTBASEPROGRAM_H_
#define VSTBASEPROGRAM_H_

// std headers
#include <vector>
#include <map>
#include <string>

// vst headers
#include <audioeffectx.h>

typedef std::map<VstInt32, float> ParameterMap;

namespace eLibV2
{
    namespace VSTBase
    {
        class VSTBaseProgram
        {
        public:
            VSTBaseProgram();
            ~VSTBaseProgram() {}

            void setParameter(const VstInt32 parameterIndex, const float value);
            float getParameter(const VstInt32 parameterIndex) const;

            void setName(const std::string name) { mProgramName = name; }
            std::string getName() { return mProgramName; }

        private:
            ParameterMap mParameters;
            std::string mProgramName;
        };

        typedef std::vector<VSTBaseProgram> VSTBasePrograms;
    }
}

#endif /* VSTBASEPROGRAM_H_ */