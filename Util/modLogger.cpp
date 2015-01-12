#include <Util/modLogger.h>

using namespace eLibV2;

bool ModuleLogger::debugActive[ModuleLogger::DEBUG_CLASS_END] = { false, false, false };
int ModuleLogger::debugType = ModuleLogger::DEBUG_TYPE_DBGVIEW;

void ModuleLogger::print(const char* format, ...)
{
	if (!debugActive[0])
		return;

	char debugBuffer[DEBUG_BUFFER_LEN];
	va_list args;

	va_start(args, format);
	vsnprintf(debugBuffer, sizeof(debugBuffer), format, args);
	if (debugType == ModuleLogger::DEBUG_TYPE_DBGVIEW)
		OutputDebugString(debugBuffer);
	else if (debugType == ModuleLogger::DEBUG_TYPE_CONSOLE)
		std::cout << debugBuffer << std::endl;

	va_end(args);
}