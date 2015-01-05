#ifndef MODBASENAME_H_
#define MODBASENAME_H_

#include <Util/modLogger.h>

#include <string>

namespace eLibV2
{
	class BaseName
	{
	public:
		BaseName(std::string name = "not initialized") : mModuleName(name) {}
		virtual std::string getModuleName(void) { return mModuleName; }

	private:
		const std::string mModuleName;
	};
}

#endif
