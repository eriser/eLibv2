#ifndef __LOCK_H__
#define __LOCK_H__

#include <Util/Threads/Mutex.h>

#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())

namespace eLibV2
{
    namespace Util
    {
        namespace Threads
        {
            class Lock
            {
            public:
                //the default constructor
                Lock(Mutex &mutex) : m_mutex(mutex), m_locked(true)
                {
                    mutex.lock();
                }

                //the destructor
                ~Lock()
                {
                    m_mutex.unlock();
                }

                //report the state of locking when used as a boolean
                operator bool() const
                {
                    return m_locked;
                }

                //unlock
                void setUnlock()
                {
                    m_locked = false;
                }

            private:
                Mutex &m_mutex;
                bool m_locked;
            };
        }
    }
}

#endif
