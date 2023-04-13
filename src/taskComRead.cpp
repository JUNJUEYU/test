#include "taskComRead.h"
#include "log.h"
#include "serialPort.h"
#include <sys/epoll.h>
#include <unistd.h>


void *taskComRead(void *arg)
{
    SerialPort *pSerialPort = new SerialPort;
    pSerialPort->openPort("/dev/ttyUSB0");
    pSerialPort->setOpt(115200, 8, 'N', 1);

    for(;;){
        if(!pSerialPort->waitData()){
            Log::print("wait data timeout");
            continue;
        }

        vector<uint8_t> data;
        pSerialPort->readData(data);
    }
}

/* ----- End of file ----- */
