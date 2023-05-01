#pragma once
#include "sql.h"

#define DB_PATH "/root/.db/Base.db"

class Db{
    public:
        Db():dbPath_m(DB_PATH){
            init();
        };
        ~Db(){};
        bool insertDev(const string &sn, const string &name);
        bool registDev(uint32_t nid, uint16_t zid, const string &sn);
        bool updateDev(uint32_t nid, uint32_t lastTime);
        bool delDev(const string &sn);
        bool delRegDev(uint32_t nid);
        bool getDevInfo(uint32_t nid, string &sn, uint16_t &zid, uint32_t &lastTime);
        bool isRegisted(uint32_t nid);
        bool isLegalDev(const string &sn);
        uint32_t getDevCount();
        string getDevName(const string &sn);
        bool randomNid(uint32_t &nid);
        bool randomZid(uint16_t &zid);
        string getGwSn();

        bool isRegLegal(const string &pd, const string &reg, const string &chan);
        uint8_t getRegParamCount(const string &pd, const string &reg, const string &chan);
        string getRegParamType(const string &pd, const string &reg, const string &chan, uint8_t index);

        bool isRegInEvent(const string &pd, const string &reg, const string &chan);
        uint8_t getEventCount(const string &pd, const string &reg, const string &chan);
        string getEventId(const string &pd, const string &reg, const string &chan, uint8_t index);
        uint8_t getEventObjCount(const string &pd, const string &eventId);


    private:
        bool init();

    private:
        string dbPath_m;
};


/* ----- End of file ----- */

