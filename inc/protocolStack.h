#pragma once
#include <vector>
#include <string>
#include <stdint.h>
#include "msg.h"
using namespace std;

class ZigNetLayer
{
    public:
        ZigNetLayer(const vector<uint8_t> &data);
        ZigNetLayer(const vector<uint8_t> &data, uint16_t zid);
        vector<uint8_t> getData();
        uint16_t getZid();
        bool unpack();

    private:
        vector<uint8_t> rawData_m;
        vector<uint8_t> outData_m;
        uint16_t zid_m;
};

class ZigTransLayer
{
    public:
        ZigTransLayer(const vector<uint8_t> &data);
        ZigTransLayer(const vector<uint8_t> &data,
                             uint16_t ver,
                             uint32_t tarId,
                             uint32_t srcId);
        vector<uint8_t> getData();
        uint32_t getSrcNid();
        uint32_t getTarNid();
        uint16_t getProtVer();
        bool unpack();

    private:
        bool integrity();

    private:
        vector<uint8_t> rawData_m;
        vector<uint8_t> outData_m;
        uint32_t tarNid_m;
        uint32_t srcNid_m;
        uint16_t protVer_m;
};

class ZigAppLayer
{
    public:
        ZigAppLayer(const vector<uint8_t> &);
        ZigAppLayer(uint8_t, uint32_t);
        ZigAppLayer(uint8_t, uint32_t, uint16_t);
        vector<uint8_t> getData();
        string getType();
        string getSn(){
            return sn_m;
        }

    private:
        string sn_m;
        uint16_t zid_m;
        uint8_t type_m;
        uint8_t subType_m;
        vector<uint8_t> outData_m;
};

class ProtocolStack
{
    public:
        Msg *getDeviceMsg(const vector<uint8_t> &);
        Msg *getDeviceMsg(const string &);

    private:
        vector<uint8_t> packNetAccessAck(uint8_t, uint32_t);
        vector<uint8_t> packNetAccessAck(uint8_t, uint32_t, uint16_t);
        MsgDev *devAccessNet(ZigAppLayer &appLayer, uint16_t ver, const vector<uint8_t> &src);

};

/* ----- End of file ----- */
