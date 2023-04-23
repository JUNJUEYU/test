#include "db.h"
#include <memory>
#include <fstream>
#include "cJSON.h"

#define DEV_PATH "/root/gwCore/devlist.json"
#define SN_PATH "/root/.db/sn"
#define SN_LEN 9

static bool initNidZidPool()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists nid_pool(nid integer primary key)";
    Stmt stmt(sql, str);
    if(stmt.step() != SQLITE_DONE){
        log_e("create table nid_pool failed");
        return false;
    }

    for (size_t i = 0; i < 200; i++)
    {
        string sqlStr = "insert into nid_pool(nid) values(?)";
        Stmt stmt1(sql, sqlStr);
        stmt1.bindInt(1, 0x8001 + i);
        if(stmt1.step() != SQLITE_DONE){
            log_e("insert nid_zid_pool failed");
            return false;
        }
    }

    string str1 = "create table if not exists zid_pool(zid integer primary key)";
    Stmt stmt1(sql, str1);
    if(stmt1.step() != SQLITE_DONE){
        log_e("create table zid_pool failed");
        return false;
    }

    for (size_t i = 0; i < 200; i++)
    {
        string sqlStr = "insert into zid_pool(zid) values(?)";
        Stmt stmt2(sql, sqlStr);
        stmt2.bindInt(1, 0x8001 + i);
        if(stmt2.step() != SQLITE_DONE){
            log_e("insert zid_pool failed");
            return false;
        }
    }

    return true;
}

static bool initMyself()
{
    FILE *fp = fopen(SN_PATH, "r");
    if(!fp){
        log_e("fopen failed");
        return false;
    }
    char sn[32] = {0};
    if(fgets(sn, SN_LEN, fp) == NULL){
        log_e("fgets failed");
        return false;
    }
    Sql sql(DB_PATH);
    string str = "create table if not exists basic(num integer primary key, sn text, name text)";
    Stmt stmt(sql, str);
    if(stmt.step() != SQLITE_DONE){
        log_e("create table basic failed");
        return false;
    }

    string sqlStr = "insert into basic(num, sn, name) values(?, ?, ?)";
    Stmt stmt1(sql, sqlStr);
    stmt1.bindInt(1, 1);
    stmt1.bindText(2, sn);
    stmt1.bindText(3, "TG-10_N");
    if(stmt1.step() != SQLITE_DONE){
        log_e("insert basic failed");
        return false;
    }
    return true;
}

static bool initDevList()
{
    ifstream in(DEV_PATH);
    if(!in){
        log_e("open devList.json failed");
        return false;
    }
    string str((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cJSON *root = cJSON_Parse(str.c_str());
    if(!root){
        log_e("cJSON_Parse failed");
        return false;
    }

    cJSON *devList = cJSON_GetObjectItem(root, "devlist");
    if(!devList){
        log_e("cJSON_GetObjectItem failed");
        return false;
    }

    int size = cJSON_GetArraySize(devList);
    if(size <= 0){
        log_e("cJSON_GetArraySize failed");
        return false;
    }
    log_d("devList size = %d", size);

    Sql sql(DB_PATH);
    string str1 = "create table if not exists dev(sn text primary key, name text)";
    Stmt stmt(sql, str1);
    if(stmt.step() != SQLITE_DONE){
        log_e("create table dev failed");
    }

    for(int i = 0; i < size; i++){
        string sqlStr = "insert into dev(sn, name) values(?, ?)";
        Stmt stmt1(sql, sqlStr);
        cJSON *item = cJSON_GetArrayItem(devList, i);
        if(!item){
            log_e("cJSON_GetArrayItem failed");
            return false;
        }
        cJSON *sn = cJSON_GetObjectItem(item, "sn");
        if(!sn){
            log_e("cJSON_GetObjectItem failed");
            return false;
        }
        cJSON *name = cJSON_GetObjectItem(item, "name");
        if(!name){
            log_e("cJSON_GetObjectItem failed");
            return false;
        }
        stmt1.bindText(1, sn->valuestring);
        stmt1.bindText(2, name->valuestring);
        if(stmt1.step() != SQLITE_DONE){
            log_e("insert dev failed");
        }
    }
    return true;
}


bool Db::init()
{
    static bool init = false;
    if(init){
        return true;
    }

    initMyself();
    initDevList();
    initNidZidPool();

    Sql sql(dbPath_m);
    string registDev = "create table if not exists registDev(nid integer primary key, sn text, name text, zid integer, lastTime integer)";
    Stmt stmt(sql, registDev);
    if(stmt.step() != SQLITE_DONE){
        log_e("create table registDev failed");
        return false;
    }
    log_e("new Sql success");
    init = true;
    return true;
}

bool Db::registDev(uint32_t nid, uint16_t zid, const string &sn)
{
    Sql sql(dbPath_m);
    string name = getDevName(sn);
    if (name.empty()){
        log_e("getDevName failed");
        return false;
    }
    
    string sqlStr = "insert into registDev(nid, sn, name, zid, lastTime) values(?, ?, ?, ?, ?)";
    Stmt stmt(sql, sqlStr);
    stmt.bindInt(1, nid);
    stmt.bindText(2, sn);
    stmt.bindText(3, name);
    stmt.bindInt(4, zid);
    stmt.bindInt(5, UtcTime::getUtcTimeStamp());
    if(stmt.step() != SQLITE_DONE){
        log_e("insert into registDev failed");
        return false;
    }

    return true;
}

bool Db::insertDev(const string &sn, const string &name)
{
    Sql sql(dbPath_m);
    string sqlStr = "insert into dev(sn, name) values(?, ?)";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, sn);
    stmt.bindText(2, name);
    if(stmt.step() != SQLITE_DONE){
        log_e("insert into dev failed");
        return false;
    }

    return true;
}

bool Db::updateDev(uint32_t nid, uint32_t lastTime)
{
    Sql sql(dbPath_m);
    string sqlStr = "update registDev set lastTime = ? where nid = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindInt(1, lastTime);
    stmt.bindInt(2, nid);
    if(stmt.step() != SQLITE_DONE){
        log_e("update registDev failed");
        return false;
    }
}


string Db::getDevName(const string &sn)
{
    Sql sql(dbPath_m);
    string sqlStr = "select name from dev where sn = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, sn);
    if(stmt.step() == SQLITE_ROW){
        return stmt.getText(0);
    }

    return "";
}

bool Db::delDev(const string &sn)
{
    Sql sql(dbPath_m);
    string sqlStr = "delete from dev where sn = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, sn);
    if(stmt.step() != SQLITE_DONE){
        log_e("delete from dev failed");
        return false;
    }

    return true;
}

bool Db::delRegDev(uint32_t nid)
{
    Sql sql(dbPath_m);
    string sqlStr = "delete from registDev where nid = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindInt(1, nid);
    if(stmt.step() != SQLITE_DONE){
        log_e("delete from registDev failed");
        return false;
    }

    return true;
}

bool Db::getDevInfo(uint32_t nid, string &sn, uint16_t &zid, uint32_t &lastTime)
{
    Sql sql(dbPath_m);
    string sqlStr = "select sn, zid, lastTime from registDev where nid = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindInt(1, nid);
    if(stmt.step() == SQLITE_ROW){
        sn = stmt.getText(0);
        zid = stmt.getInt(1);
        lastTime = stmt.getInt(2);
        return true;
    }

    return false;
}

bool Db::isRegisted(uint32_t nid)
{
    Sql sql(dbPath_m);
    string sqlStr = "select nid from registDev where nid = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindInt(1, nid);
    if(stmt.step() == SQLITE_ROW){
        return true;
    }

    return false;
}

bool Db::isLegalDev(const string &sn)
{
    Sql sql(dbPath_m);
    string sqlStr = "select sn from dev where sn = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, sn);
    if(stmt.step() == SQLITE_ROW){
        return true;
    }

    return false;
}

uint32_t Db::getDevCount()
{
    Sql sql(dbPath_m);
    string sqlStr = "select count(*) from dev";
    Stmt stmt(sql, sqlStr);
    if(stmt.step() == SQLITE_ROW){
        return stmt.getInt(0);
    }

    return 0;
}

bool Db::randomNid(uint32_t &nid)
{
    Sql sql(dbPath_m);
    string sqlStr = "select nid from nidlist";
    Stmt stmt(sql, sqlStr);
    if(stmt.step() != SQLITE_ROW){
        
        return false;
    }

    nid = stmt.getInt(0);
    string sqlStr1 = "delete from nidlist where nid = ?";
    Stmt stmt1(sql, sqlStr1);
    stmt1.bindInt(1, nid);
    if(stmt1.step() != SQLITE_DONE){
        log_e("delete from nidlist failed");
        return false;
    }

    return true;
}

bool Db::randomZid(uint16_t &zid)
{
    Sql sql(dbPath_m);
    string sqlStr = "select zid from zidlist";
    Stmt stmt(sql, sqlStr);
    if(stmt.step() == SQLITE_ROW){
        return false;
    }
    zid = stmt.getInt(0);
    string sqlStr1 = "delete from zidlist where zid = ?";
    Stmt stmt1(sql, sqlStr1);
    stmt1.bindInt(1, zid);
    if(stmt1.step() != SQLITE_DONE){
        log_e("delete from zidlist failed");
        return false;
    }

    return true;
}

string Db::getGwSn()
{
    Sql sql(dbPath_m);
    string sqlStr = "select sn from basic";
    Stmt stmt(sql, sqlStr);
    if(stmt.step() == SQLITE_ROW){
        return stmt.getText(0);
    }

    return "";
}


/* ----- End of file ----- */
