#include "eventManager.h"
#include "log.h"
#include "tools.h"

EventManager::EventManager()
{
    log_d("EventManager::EventManager");
}

EventManager::~EventManager()
{;}

void EventManager::addEvent(Event *event)
{
    Lock lock(mutex_m);
    log_d("EventManager::addEvent");
    eventList_m.push_back(event);
}

void EventManager::delEvent(Event *event)
{
    Lock lock(mutex_m);
    log_d("EventManager::delEvent");
    eventList_m.remove(event);
}

void EventManager::delEvent(const string &uuid)
{
    Lock lock(mutex_m);
    log_d("EventManager::delEvent");
    list<Event *>::iterator it;
    for (it = eventList_m.begin(); it != eventList_m.end(); it++)
    {
        if ((*it)->getUuid() == uuid)
        {
            eventList_m.erase(it);
            break;
        }
    }
}

void EventManager::pendEvent(Event *event, uint32_t timeout)
{
    Lock lock(mutex_m);
    log_d("EventManager::pendingEvent");
    pair<Event *, uint32_t> eventPair(event, timeout);
    pendingList_m.push_back(eventPair);
}

void EventManager::delPendEvent(Event *event)
{
    Lock lock(mutex_m);
    log_d("EventManager::delPendEvent");
    list<pair<Event *, uint32_t> >::iterator it;
    for (it = pendingList_m.begin(); it != pendingList_m.end(); it++)
    {
        if ((*it).first == event)
        {
            pendingList_m.erase(it);
            log_d("EventManager::delPendEvent: delete event");
            break;
        }
    }
}

Event *EventManager::getPendEvent(const string &uuid)
{
    Lock lock(mutex_m);
    log_d("EventManager::getPendEvent");
    list<pair<Event *, uint32_t> >::iterator it;
    for (it = pendingList_m.begin(); it != pendingList_m.end(); it++)
    {
        if ((*it).first->getUuid() == uuid)
        {
            return (*it).first;
        }
    }
    return NULL;
}


/* ----- End of file ----- */
