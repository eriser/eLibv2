#include <Util/modLogger.h>

bool ModuleLogger::debugActive[DBG_CLASS_END] = { false, false, false };

void ModuleLogger::print(const char* format, ...)
{
    if (!debugActive[0])
        return;

    char debugBuffer[BUFFER_LEN];
    va_list args;

    va_start(args, format);
    vsnprintf(debugBuffer, sizeof(debugBuffer), format, args);
    OutputDebugString(debugBuffer);
    va_end(args);
}