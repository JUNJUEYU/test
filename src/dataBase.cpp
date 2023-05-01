#include "dataBase.h"

DataBase::DataBase(string pd, string sn, string reg, string chan):
    pd_m(pd), sn_m(sn), reg_m(reg), chan_m(chan)
{

}

DataBase::~DataBase()
{

}

void DataBase::addParam(DataType_t type, string data)
{
    param_m.push_back(make_pair(type, data));
}

bool DataBase::getParam(uint32_t index, string &data)
{
    if(index >= param_m.size())
        return false;
    data = param_m[index].second;
    return true;
}

bool DataBase::getParamType(uint32_t index, DataType_t &type)
{
    if(index >= param_m.size())
        return false;
    type = param_m[index].first;
    return true;
}