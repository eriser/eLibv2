/*
* modBaseName.h
*
*  Created on: 25.11.2011
*      Author: dedokter
*/

#ifndef MODBASENAME_H_
#define MODBASENAME_H_

#include <Util/modLogger.h>

#include <string>

namespace eLibV2
{
	class BaseName
	{
	public:
		BaseName() { ModuleLogger::print("wdwdadadaw"); setModuleName("Baseame"); }
		virtual std::string getModuleName(void) { return mModuleName; }
		virtual void setModuleName(std::string ModuleName) { mModuleName.assign(ModuleName); }

	private:
		std::string mModuleName;
	};
}

#endif
