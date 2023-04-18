#pragma once
#include <vector>
#include <stdint.h>
using namespace std;

class SerialPort
{
    public:
        SerialPort();
        ~SerialPort();
        int openPort(const char *devName);
        int setOpt(int nSpeed, int nBits, char nEvent, int nStop);
        int readData(vector<uint8_t> &buf);
        int writeData(const vector<uint8_t>& buf);
        bool waitData();
        void closePort();
    private:
        int serialFd_m;
        int epollFd_m;
};

/* ----- End of file ----- */
