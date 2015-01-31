#ifndef PLUGINPROGRAM_H_
#define PLUGINPROGRAM_H_

// std headers
#include <vector>
#include <map>
#include <string>

namespace eLibV2
{
	class PluginProgram
	{
	public:
		PluginProgram(const unsigned long numParameters) { mNumParameters = numParameters; }
		~PluginProgram() {}

		void addParameter(const float value);
		void setParameter(const unsigned long parameterIndex, const float value);
		float getParameter(const unsigned long parameterIndex) const;

		void setName(const std::string name) { mProgramName = name; }
		std::string getName() const { return mProgramName; }

	private:
		unsigned long mNumParameters;
		std::vector<float> mParameterValues;
		std::string mProgramName;
	};

	typedef std::vector<PluginProgram> PluginPrograms;
}

#endif
