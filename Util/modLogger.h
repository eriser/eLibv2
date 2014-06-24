#ifndef MODLOGGER_H_
#define MODLOGGER_H_

#include <windows.h>
#include <stdio.h>

#define BUFFER_LEN  1024

class ModuleLogger
{
public:
    static void setActive(bool value) { debugActive = value; }
    static void print(const char* format, ...);

private:
    ModuleLogger();
    static bool debugActive;
};

#endif /* MODLOGGER_H_ */
