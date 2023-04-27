#pragma once
#include <string>
#include <vector>
#include <stdint.h>
using namespace std;

class Property{
    public:
        string name_m;
        uint8_t offest_m;
        string dataType_m;
};


class RegisterDesc{
    public:
        uint16_t regAddr_m;
        string rw_m;
        vector<Property> proList_m;
};


class RegisterDescTbl{
    public:
        RegisterDescTbl(const string &path);
        bool getRegDesc(uint16_t regAddr, RegisterDesc &regDesc);
        int getRegDescSize();
        
    private:
        string pd_m;
        vector<RegisterDesc> regList_m;
};
