#ifndef MODBASENAME_H_
#define MODBASENAME_H_

#include <Util/modLogger.h>

#include <string>

namespace eLibV2
{
	class BaseName
	{
	public:
		BaseName() { setModuleName("not initialized"); }
		BaseName(std::string name) { setModuleName(name); }
		virtual std::string getModuleName(void) { return mModuleName; }
		virtual void setModuleName(std::string ModuleName) { mModuleName.assign(ModuleName); }

	private:
		std::string mModuleName;
	};
}

#endif
