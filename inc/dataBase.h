#pragma once
#include <string>
#include <vector>
#include <utility>
#include <stdint.h>
using namespace std;
typedef enum{
    uint8_e,
    uint16_e,
    uint32_e,
    int32_e,
    float32_e,
    text_e,
    false_e
}DataType_t;

typedef pair<DataType_t, string> DataPair_t;

class DataBase{
    private:
        string pd_m;
        string sn_m;
        string reg_m;
        string chan_m;
        vector<DataPair_t> param_m;
    public:
        DataBase(string pd, string sn, string reg, string chan);
        DataBase(){};
        ~DataBase();
        void addParam(DataType_t type, string data);
        bool getParam(uint32_t index, string &data);
        bool getParamType(uint32_t index, DataType_t &type);
        string getPd(){return pd_m;};
        string getSn(){return sn_m;};
        string getReg(){return reg_m;};
        string getChan(){return chan_m;};
};
