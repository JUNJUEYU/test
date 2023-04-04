#pragma once
#include <string>
#include <vector>
#include <stdint.h>

using namespace std;

class pluginProcAPI
{
private:
    /* data */
public:
    virtual ~pluginProcAPI(){};
    virtual string trans(const vector<uint8_t> &) = 0;
    virtual vector<uint8_t> trans(const string &) = 0;
};

pluginProcAPI::pluginProcAPI(/* args */)
{
}

pluginProcAPI::~pluginProcAPI()
{
}

/* ----- End of file ----- */
