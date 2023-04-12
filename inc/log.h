#pragma once
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;



class Log{
    public:
    static void print(const char *msg);
    static void print(const char *msg, int value);
    static void print(const char *msg, vector<uint8_t> value);
};

/* ----- End of file ----- */
