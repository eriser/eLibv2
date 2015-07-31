#ifndef MODLOGGER_H_
#define MODLOGGER_H_

#include <windows.h>

#include <cstdio>
#include <iostream>

namespace eLibV2
{
    namespace Util
    {
        class ModuleLogger
        {
        public:
            enum LoggingClass
            {
                LOGGING_CLASS_NONE = 0,
                LOGGING_CLASS_1 = 0x00000001,
                LOGGING_CLASS_2 = 0x00000002,
                LOGGING_CLASS_3 = 0x00000004,
                LOGGING_CLASS_4 = 0x00000008,
                LOGGING_CLASS_5 = 0x00000010,
                LOGGING_CLASS_6 = 0x00000020,
                LOGGING_CLASS_7 = 0x00000040,
                LOGGING_CLASS_8 = 0x00000080,
                LOGGING_CLASS_9 = 0x00000100,
                LOGGING_CLASS_10 = 0x00000200,
                LOGGING_CLASS_11 = 0x00000400,
                LOGGING_CLASS_12 = 0x00000800,
                LOGGING_CLASS_13 = 0x00001000,
                LOGGING_CLASS_14 = 0x00002000,
                LOGGING_CLASS_15 = 0x00004000,
                LOGGING_CLASS_16 = 0x00008000,
                LOGGING_CLASS_17 = 0x00010000,
                LOGGING_CLASS_18 = 0x00020000,
                LOGGING_CLASS_19 = 0x00040000,
                LOGGING_CLASS_20 = 0x00080000,
                LOGGING_CLASS_21 = 0x00100000,
                LOGGING_CLASS_22 = 0x00200000,
                LOGGING_CLASS_23 = 0x00400000,
                LOGGING_CLASS_24 = 0x00800000,
                LOGGING_CLASS_25 = 0x01000000,
                LOGGING_CLASS_26 = 0x02000000,
                LOGGING_CLASS_27 = 0x04000000,
                LOGGING_CLASS_28 = 0x08000000,
                LOGGING_CLASS_29 = 0x10000000,
                LOGGING_CLASS_30 = 0x20000000,
                LOGGING_CLASS_31 = 0x40000000,
                LOGGING_CLASS_32 = 0x80000000,
                LOGGING_CLASS_ALL = 0xFFFFFFFF
            };

            enum OutputType
            {
                OUTPUT_TYPE_CONSOLE,
                OUTPUT_TYPE_DBGVIEW,
            };

            /**
            Enable all logging classes, so every message will be printed
            */
            static void enable(void) { EnableLoggingClasses(LOGGING_CLASS_ALL); }

            /**
            Enable given logging classes
            @param classes the logging classes to activate
            */
            static void enable(unsigned int classes) { EnableLoggingClasses(loggingClasses | classes); }

            /**
            Disable all logging classes, so no message will be printed
            */
            static void disable(void) { EnableLoggingClasses(LOGGING_CLASS_NONE); }

            /**
            Disable given logging classes
            @param classes the logging classes to deactivate
            */
            static void disable(unsigned int classes) { EnableLoggingClasses(loggingClasses & ~classes); }

            /**
            All outputs will be sent to the console
            */
            static void setConsole(void) { outputType = OUTPUT_TYPE_CONSOLE; }

            /**
            All output will be sent to debugging output
            */
            static void setDebugView(void) { outputType = OUTPUT_TYPE_DBGVIEW; }

            /**
            Print message
            @param loggingClass class set for message
            @param format format of message according to printf
            */
            static void print(unsigned int loggingClass, const char* format, ...);

        private:
            static void EnableLoggingClasses(unsigned int classes) { loggingClasses = classes; }
            static bool LoggingClassesEnabled(unsigned int classes) { return (loggingClasses & classes); }

        private:
            ModuleLogger();
            static unsigned int loggingClasses;
            static OutputType outputType;

            static const int DEBUG_BUFFER_LEN = 1024;
        };
    }
}

#endif
