#pragma once 
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdint.h>
#include "paramBase.h"
using namespace std;


class Property
{
    public:
        string name_m;
        string rw_m;
        uint32_t regAddr_m;
        vector<ParamBase> param_m;
};


class PropertyTable
{
    public:
        static PropertyTable& getInstance();
        void addProperty(Property cmd);
        bool getProperty(string name, Property &cmd);
        bool getProperty(uint32_t regAddr, Property &cmd);
        int getPropertySize();
    private:
        PropertyTable();
        PropertyTable(const PropertyTable&);
        PropertyTable& operator=(const PropertyTable&);
        vector<Property> proList_m;
};