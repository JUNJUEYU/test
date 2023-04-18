#include "task.h"
#include "eventInterface.h"
#include <memory>


void *task(void *arg)
{
    if(arg == NULL){
        return NULL;
    }

    auto_ptr<Event> event((Event *)arg);
    event->run();
    return NULL;
}

/* ----- End of file ----- */
