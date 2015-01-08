#ifndef MODLOGGER_H_
#define MODLOGGER_H_

#include <windows.h>
#include <stdio.h>

namespace eLibV2
{
	class ModuleLogger
	{
	public:
		enum { BUFFER_LEN = 1024 };
		enum
		{
			DBG_CLASS_START = 0,
			DBG_CLASS_START1,
			DBG_CLASS_START2,
			DBG_CLASS_END
		};

		static void setActive(bool value) { if (debugActive[0] != value) debugActive[0] = value; }
		static void print(const char* format, ...);

	private:
		ModuleLogger();
		static bool debugActive[DBG_CLASS_END];
	};
}

#endif
