#include "serialPort.h"
#include "log.h"
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

SerialPort::SerialPort():serialFd_m(-1),epollFd_m(-1)
{
}

SerialPort::~SerialPort()
{
    closePort();
}

int SerialPort::openPort(const char *devName)
{
    serialFd_m = open(devName, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if (serialFd_m < 0)
    {
        return -1;
    }

    epollFd_m = epoll_create(1); 
    struct epoll_event ev, events[1];
    ev.data.fd = serialFd_m;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollFd_m, EPOLL_CTL_ADD, serialFd_m, &ev);

    return 0;
}

int SerialPort::setOpt(int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio, oldtio;
    if (tcgetattr(serialFd_m, &oldtio) != 0)
    {
        log_d("tcgetattr failed");
        return -1;
    }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch (nBits)
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch (nEvent)
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch (nSpeed)
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    case 460800:
        cfsetispeed(&newtio, B460800);
        cfsetospeed(&newtio, B460800);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }

    if (nStop == 1)
    {
        newtio.c_cflag &= ~CSTOPB;
    }
    else if (nStop == 2)
    {
        newtio.c_cflag |= CSTOPB;
    }

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(serialFd_m, TCIFLUSH);
    if ((tcsetattr(serialFd_m, TCSANOW, &newtio)) != 0)
    {
        log_d("tcsetattr failed");
        return -1;
    }
    log_d("setOpt success");
    return 0;
}

int SerialPort::readData(vector<uint8_t> &outBuf)
{
    uint8_t buf[1024] = {0};
    int ret = read(serialFd_m, buf, 200);
    if (ret < 0)
    {
        log_d("read failed");
        return -1;
    }
    outBuf = vector<uint8_t>(buf, buf + ret);
    return ret;
}

int SerialPort::writeData(const vector<uint8_t> &buf)
{
    int ret = write(serialFd_m, buf.data(), buf.size());
    if (ret < 0)
    {
        log_d("send failed");
        return -1;
    }
    return ret;
}

void SerialPort::closePort()
{
    if (serialFd_m > 0)
    {
        close(serialFd_m);
        serialFd_m = -1;
    }
}

bool SerialPort::waitData()
{
    struct epoll_event events[1];
    int nfds = epoll_wait(epollFd_m, events, 1, -1);
    if (nfds == 0)
    {
        return false;
    }
    else if (nfds < 0)
    {
        log_d("epoll_wait failed");
        return false;
    }
    else
    {
        return true;
    }
}


/* ----- End of file ----- */
