#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "elog.h"

using namespace std;

#define LOG_FILE "/root/log/iot.log"
#define LOG_SIZE 1024*1024*10
#define LOG_COUNT 10

class Log{
    public:
    static void init();
    static void print(const char *msg);
    static void print(const char *msg, int value);
    static void print(const char *msg, const char *value);
    static void print(const char *msg, vector<uint8_t> value);
    static void print(const char *msg, const string & value);

    static void error(const char *msg);
    static void error(const char *msg, int value);
    static void error(const char *msg, const char *value);
    static void error(const char *msg, vector<uint8_t> value);
};

/* ----- End of file ----- */
