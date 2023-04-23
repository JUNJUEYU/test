#pragma once 
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdint.h>
using namespace std;


class ValueBase{
    public:
        string type;
        uint8_t uint8;
        uint16_t uint16;
        uint32_t uint32;
        int32_t int32;
        float float32;
        string text;
};

class ParamBase{
    public:
        string name;
        uint8_t num;   
        string type;
};