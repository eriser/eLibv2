#ifndef MODLOGGER_H_
#define MODLOGGER_H_

#include <windows.h>
#include <stdio.h>

#include <iostream>

namespace eLibV2
{
    namespace Util
    {
        class ModuleLogger
        {
        public:
            enum DebugClass
            {
                DEBUG_CLASS_START = 0,
                DEBUG_CLASS_START1,
                DEBUG_CLASS_START2,
                DEBUG_CLASS_END
            };

            enum OutputType
            {
                OUTPUT_TYPE_CONSOLE,
                OUTPUT_TYPE_DBGVIEW,
            };
            static void enable(void) { setActive(true); }
            static void disable(void) { setActive(false); }
            static void setConsole(void) { debugType = OUTPUT_TYPE_CONSOLE; }
            static void setDebugView(void) { debugType = OUTPUT_TYPE_DBGVIEW; }
            static void print(const char* format, ...);

        private:
            static void setActive(bool value) { if (debugActive[0] != value) debugActive[0] = value; }

        private:
            ModuleLogger();
            static bool debugActive[DEBUG_CLASS_END];
            static int debugType;

            static const int DEBUG_BUFFER_LEN = 1024;
        };
    }
}

#endif
