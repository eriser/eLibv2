#ifndef MODDEFINES_H_
#define MODDEFINES_H_

#ifndef COMPANY_NAME
#define COMPANY_NAME "SyS AudioResearch"
#endif

#ifndef PI
#define PI 3.1415926535
#endif

#ifndef PI_DIV_2
#define PI_DIV_2 1.570796
#endif

#ifndef SQRT_2
#define SQRT_2 1.414214
#endif

#ifndef MIDISCALER
#define MIDISCALER 1.0 / 127.0
#endif

#ifndef EULER
#define EULER 2.71828
#endif

#ifndef EULER_DIV_2
#define EULER_DIV_2 1.95716
#endif

#ifndef LOWEST
#define LOWEST 0.0001
#endif

#define DEBUG_MSG_MAX       1024

#ifndef sgn
#define sgn(x) x < 0.0 ? -1.0 : 1.0
#endif

#ifndef FLT_MIN_PLUS
#define FLT_MIN_PLUS    1.175494351e-38
#endif

#ifndef FLT_MIN_MINUS
#define FLT_MIN_MINUS  -1.175494351e-38
#endif

#define LOG_CLASS_ELIB          0x10000000
#define LOG_CLASS_ASIO          LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_1
#define LOG_CLASS_CONNECTION    LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_2
#define LOG_CLASS_DATA          LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_3
#define LOG_CLASS_EFFECT        LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_4
#define LOG_CLASS_GENERATOR     LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_5
#define LOG_CLASS_LOADER        LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_6
#define LOG_CLASS_MIDI          LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_7
#define LOG_CLASS_PLUGIN        LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_8
#define LOG_CLASS_UTIL          LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_9
#define LOG_CLASS_VSTBASE       LOG_CLASS_ELIB | ModuleLogger::LOGGING_CLASS_10

#endif