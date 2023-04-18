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
        ZigAppLayer(uint16_t type, uint16_t msgID, const vector<uint8_t> &data);
        vector<uint8_t> getData();
        uint16_t getType();
        uint16_t getMsgID();

    private:
        uint16_t zid_m;
        uint16_t type_m;
        uint16_t msgID_m;
        vector<uint8_t> outData_m;
};

class zigProtStack{
    public:
        zigProtStack(const vector<uint8_t> &data);
        ~zigProtStack();

        vector<uint8_t> getData();
        uint16_t getZid();
        uint32_t getSrcNid();
        uint32_t getTarNid();
        uint16_t getProtVer();
        uint16_t getType();
        uint16_t getMsgID(){
            return msgID_m;
        };
        bool parse();

        void setZid(uint16_t zid);
        void setProtVer(uint16_t protVer);
        void setSrcNid(uint32_t srcNid);
        void setTarNid(uint32_t tarNid);
        void setType(uint16_t type);
        void setMsgID(uint16_t msgID);
        void setParam(uint16_t zid, uint16_t protVer, uint32_t srcNid, uint32_t tarNid, uint16_t type, uint16_t msgID){
            zid_m = zid;
            protVer_m = protVer;
            srcNid_m = srcNid;
            tarNid_m = tarNid;
            type_m = type;
            msgID_m = msgID;
            pack();
        };
        bool pack();
    
    private:
        vector<uint8_t> rawData_m;
        vector<uint8_t> data_m;
        uint16_t zid_m;
        uint32_t srcNid_m;
        uint32_t tarNid_m;
        uint16_t protVer_m;
        uint16_t type_m;
        uint16_t msgID_m;
};

/* ----- End of file ----- */
