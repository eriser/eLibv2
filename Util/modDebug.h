#ifndef MODDEBUG_H_
#define MODDEBUG_H_

#include <Util/modDefines.h>
#include <Base/modBaseName.h>

#include <iostream>

#include <Windows.h>
#include <string>

namespace eLibV2
{
	class ModuleDebug
	{
	public:
		void TestBeginMsg(void) { TestMode = true; dbgOutput("Begin Test"); }
		void TestEndMsg(void) { TestMode = false; dbgOutput("End Test"); }

		void dbgOutputF(char *format, ...)
		{
			char MsgBuffer[DEBUG_MSG_MAX];
			va_list vl;

			va_start(vl, format);
			vsprintf(MsgBuffer, format, vl);
			dbgOutput(MsgBuffer);
			va_end(vl);
		}

		void dbgOutput(char *msg)
		{
			std::string dbgString;

			dbgString.assign("");
			dbgString.append(": ");
			dbgString.append(msg);

#if defined(WIN32) && !defined(_CONSOLE)
			OutputDebugString(dbgString.c_str());
#else
			std::cout << dbgString << std::endl;
#endif
		}

	private:
		bool TestMode;
	};
}

#endif
