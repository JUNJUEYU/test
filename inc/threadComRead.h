#pragma once
 
#include "myThread.h"

class ComReadThread:public Thread{
    public:
        ComReadThread();
        ComReadThread(const char *devName, int baudRate){};
        ~ComReadThread();

        void run();
};
/* ----- End of file ----- */
