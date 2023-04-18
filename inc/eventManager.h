#pragma once
#include "eventInterface.h"
#include <list>
#include <string>
#include <pthread.h>
#include <iostream>
using namespace std;

class EventManager
{
public:
    static EventManager &getInstance(){
        static EventManager instance;
        return instance;
    };
    void init();
    void addEvent(Event *event);
    void delEvent(Event *event);
    void delEvent(const string &uuid); 
    void pendEvent(Event *event, uint32_t timeout);
    void delPendEvent(Event *event);
    Event *getPendEvent(const string &uuid);
private:
    EventManager();
    ~EventManager();
    list<Event *> eventList_m;
    list<pair<Event *, uint32_t> > pendingList_m;
    pthread_mutex_t mutex_m;
};



/* ----- End of file ----- */
