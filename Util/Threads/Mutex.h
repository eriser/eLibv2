#ifndef __MUTEX_H__
#define __MUTEX_H__

#ifdef WIN32

#include <windows.h>

namespace eLibV2
{
    namespace Util
    {
        namespace Threads
        {
            class Mutex
            {
            public:
                //the default constructor
                Mutex()
                {
                    InitializeCriticalSection(&m_criticalSection);
                }

                //destructor
                ~Mutex()
                {
                    DeleteCriticalSection(&m_criticalSection);
                }

                //lock
                void lock()
                {
                    EnterCriticalSection(&m_criticalSection);
                }

                //unlock
                void unlock()
                {
                    LeaveCriticalSection(&m_criticalSection);
                }

            private:
                CRITICAL_SECTION m_criticalSection;
            };
        }
    }
}

#endif

#endif
