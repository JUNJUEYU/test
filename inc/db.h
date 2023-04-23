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

    private:
        bool init();

    private:
        string dbPath_m;
};


/* ----- End of file ----- */

