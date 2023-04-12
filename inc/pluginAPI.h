#pragma once
#include <string>
#include <vector>
#include <stdint.h>

using namespace std;

class ProtAPI
{
private:
    /* data */
public:
    virtual ~ProtAPI(){};
    virtual bool trans(const vector<uint8_t> &, string &) = 0;
    virtual bool trans(const string &, vector<uint8_t> &) = 0;
    virtual string getTopic() = 0;
    virtual uint16_t getVersion() = 0;
};

class PluginAPI{
    public:
        virtual ~PluginAPI(){};
        virtual ProtAPI *create() = 0;
};


/* ----- End of file ----- */
