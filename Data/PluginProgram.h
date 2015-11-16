#ifndef PLUGINPROGRAM_H_
#define PLUGINPROGRAM_H_

// std headers
#include <vector>
#include <map>
#include <string>

#include <Util/Types.h>

namespace eLibV2
{
    namespace Data
    {
        class PluginProgram
        {
        public:
            PluginProgram(const UInt16 numParameters) { mNumParameters = numParameters; }
            ~PluginProgram() {}

            void addParameter(const float value);
            void setParameter(const UInt16 parameterIndex, const float value);
            float getParameter(const UInt16 parameterIndex) const;

            void setName(const std::string name) { mProgramName = name; }
            std::string getName() const { return mProgramName; }

        private:
            UInt16 mNumParameters;
            std::vector<float> mParameterValues;
            std::string mProgramName;
        };

        typedef std::vector<PluginProgram> PluginPrograms;
    }
}

#endif
