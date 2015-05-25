#ifndef MODDEFINES_H_
#define MODDEFINES_H_

#ifndef COMPANY_NAME
#define COMPANY_NAME "SyS AudioResearch"
#endif

#ifndef PI
#define PI 3.1415926535
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

#endif