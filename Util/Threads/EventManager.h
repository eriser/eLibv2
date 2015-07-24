#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include <map>

//#define USE_EVENT_MANAGER

#ifdef WIN32

#include <Windows.h>

namespace eLibV2
{
    namespace Util
    {
        namespace Threads
        {
            class EventManager
            {
            public:
                enum Event
                {
                    EVENT_PROCESSING_DONE,
                    EVENT_DATA_WRITTEN,
                };

            public:
                static void RegisterEvent(EventManager::Event event)
                {
                    // if an event was already registered, close the handle and create a new one
                    UnregisterEvent(event);
                    EventMap[event] = CreateEvent(NULL, TRUE, FALSE, NULL);
                    ResetEvent(event);
                }

                static void UnregisterEvent(EventManager::Event event)
                {
                    if (EventMap[event])
                    {
                        ResetEvent(event);
                        CloseHandle(EventMap[event]);
                        EventMap[event] = NULL;
                    }
                }

                static bool WaitForEvent(EventManager::Event event)
                {
                    if (EventMap[event])
                    {
                        WaitForSingleObject(EventMap[event], INFINITE);
                        return true;
                    }
                    return false;
                }

                static void ResetEvent(EventManager::Event event)
                {
                    // ResetEvent collides with windows function
                    if (EventMap.count(event) > 0)
                        ::ResetEvent(EventMap[event]);
                }

                static void SetEvent(EventManager::Event event)
                {
                    // SetEvent collides with windows function
                    if (EventMap.count(event) > 0)
                        ::SetEvent(EventMap[event]);
                }

            private:
                static std::map<EventManager::Event, HANDLE> EventMap;
            };
        }
    }
}

#endif

#endif
