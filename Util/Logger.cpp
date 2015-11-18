#include <Util/Logger.h>

using namespace eLibV2::Util;

SInt32 ModuleLogger::loggingClasses = 0;
#ifdef WIN32
ModuleLogger::OutputType ModuleLogger::outputType = ModuleLogger::OUTPUT_TYPE_DBGVIEW;
#else
ModuleLogger::OutputType ModuleLogger::outputType = ModuleLogger::OUTPUT_TYPE_CONSOLE;
#endif

void ModuleLogger::print(SInt32 loggingClass, const char* format, ...)
{
    if (!LoggingClassesEnabled(loggingClass))
        return;

    char debugBuffer[DEBUG_BUFFER_LEN];
    va_list args;

    va_start(args, format);
    vsnprintf(debugBuffer, sizeof(debugBuffer), format, args);

    // append \n at the end
    snprintf(debugBuffer, sizeof(debugBuffer), "%s\n", debugBuffer);
    if (outputType == ModuleLogger::OUTPUT_TYPE_CONSOLE)
        std::cout << debugBuffer;
#ifdef WIN32
    else if (outputType == ModuleLogger::OUTPUT_TYPE_DBGVIEW)
        OutputDebugString(debugBuffer);
#endif

    va_end(args);
}