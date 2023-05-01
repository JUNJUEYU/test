#include "db.h"
#include <memory>
#include <fstream>
#include "cJSON.h"
#include <iostream>

#define PRODUCT_ID "TS-30_N"
#define REG_PATH "/root/gwCore/reg.json"
#define EVENT_PATH "/root/gwCore/event.json"
#define DEV_PATH "/root/gwCore/devlist.json"
#define SN_PATH "/root/.db/sn"
#define SN_LEN 9

static bool initNidZidPool()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists nid_pool(nid integer primary key)";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table nid_pool failed");
        return false;
    }

    for (size_t i = 0; i < 200; i++)
    {
        string sqlStr = "insert into nid_pool(nid) values(?)";
        Stmt stmt1(sql, sqlStr);
        stmt1.bindInt(1, 0x8001 + i);
        if (stmt1.step() != SQLITE_DONE)
        {
            log_e("insert nid_zid_pool failed");
            return false;
        }
    }

    string str1 = "create table if not exists zid_pool(zid integer primary key)";
    Stmt stmt1(sql, str1);
    if (stmt1.step() != SQLITE_DONE)
    {
        log_e("create table zid_pool failed");
        return false;
    }

    for (size_t i = 0; i < 200; i++)
    {
        string sqlStr = "insert into zid_pool(zid) values(?)";
        Stmt stmt2(sql, sqlStr);
        stmt2.bindInt(1, 0x8001 + i);
        if (stmt2.step() != SQLITE_DONE)
        {
            log_e("insert zid_pool failed");
            return false;
        }
    }

    return true;
}

static bool initMyself()
{
    FILE *fp = fopen(SN_PATH, "r");
    if (!fp)
    {
        log_e("fopen failed");
        return false;
    }
    char sn[32] = {0};
    if (fgets(sn, SN_LEN, fp) == NULL)
    {
        log_e("fgets failed");
        return false;
    }
    Sql sql(DB_PATH);
    string str = "create table if not exists basic(num integer primary key, sn text, name text)";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table basic failed");
        return false;
    }

    string sqlStr = "insert into basic(num, sn, name) values(?, ?, ?)";
    Stmt stmt1(sql, sqlStr);
    stmt1.bindInt(1, 1);
    stmt1.bindText(2, sn);
    stmt1.bindText(3, "TG-10_N");
    if (stmt1.step() != SQLITE_DONE)
    {
        log_e("insert basic failed");
        return false;
    }
    return true;
}

static bool initDevList()
{
    ifstream in(DEV_PATH);
    if (!in)
    {
        log_e("open devList.json failed");
        return false;
    }
    string str((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cJSON *root = cJSON_Parse(str.c_str());
    if (!root)
    {
        log_e("cJSON_Parse failed");
        return false;
    }

    cJSON *devList = cJSON_GetObjectItem(root, "devlist");
    if (!devList)
    {
        log_e("cJSON_GetObjectItem failed");
        return false;
    }

    int size = cJSON_GetArraySize(devList);
    if (size <= 0)
    {
        log_e("cJSON_GetArraySize failed");
        return false;
    }
    log_d("devList size = %d", size);

    Sql sql(DB_PATH);
    string str1 = "create table if not exists dev(sn text primary key, name text)";
    Stmt stmt(sql, str1);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table dev failed");
    }

    for (int i = 0; i < size; i++)
    {
        string sqlStr = "insert into dev(sn, name) values(?, ?)";
        Stmt stmt1(sql, sqlStr);
        cJSON *item = cJSON_GetArrayItem(devList, i);
        if (!item)
        {
            log_e("cJSON_GetArrayItem failed");
            return false;
        }
        cJSON *sn = cJSON_GetObjectItem(item, "sn");
        if (!sn)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }
        cJSON *name = cJSON_GetObjectItem(item, "name");
        if (!name)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }
        stmt1.bindText(1, sn->valuestring);
        stmt1.bindText(2, name->valuestring);
        if (stmt1.step() != SQLITE_DONE)
        {
            log_e("insert dev failed");
        }
    }
    return true;
}

static bool initNetTopTable()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists netTopo \
                    (sn text primary key, \
                    prodect_id text, \
                    connect_mode text, \
                    zid integer, \
                    uuid integer, \
                    last_time integer, \
                    sys_start_time integer, \
                    sys_update_time integer \
                    )";
    
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table netTopo failed");
        return false;
    }
    log_e("create table netTopo success");
    return true;
}

static bool initRegTable()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists reg( \
                    prodect_id text, \
                    reg_id text, \
                    chan_id text, \
                    param_count integer, \
                    sys_start_time integer, \
                    sys_update_time integer, \
                    primary key(prodect_id, reg_id, chan_id) \
                    )";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table reg failed");
        return false;
    }
    log_i("create table reg success");

    ifstream in(REG_PATH);
    if (!in)
    {
        log_e("open reg.json failed");
        return false;
    }

    string regStr((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cJSON *root = cJSON_Parse(regStr.c_str());
    if (!root)
    {
        log_e("cJSON_Parse failed");
        return false;
    }

    cJSON *reg = cJSON_GetObjectItem(root, "reg");
    if (!reg)
    {
        log_e("cJSON_GetObjectItem failed");
        return false;
    }

    int size = cJSON_GetArraySize(reg);
    if (size <= 0)
    {
        log_e("cJSON_GetArraySize failed");
        return false;
    }

    for (int i = 0; i < size; i++)
    {
        cJSON *item = cJSON_GetArrayItem(reg, i);
        if (!item)
        {
            log_e("cJSON_GetArrayItem failed");
            return false;
        }

        cJSON *addr = cJSON_GetObjectItem(item, "addr");
        if (!addr)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        cJSON *chan = cJSON_GetObjectItem(item, "chan");
        if (!chan)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        cJSON *param = cJSON_GetObjectItem(item, "param");
        if (!param)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        int paramCount = cJSON_GetArraySize(param);

        string sqlStr = "insert into reg(prodect_id, reg_id, chan_id, param_count, sys_start_time, sys_start_time) \
                        values(?, ?, ?, ?, ?, ?)";
        Stmt stmt1(sql, sqlStr);
        string prodectId = PRODUCT_ID;
        string regId = addr->valuestring;
        string chanId = chan->valuestring;
        stmt1.bindText(1, prodectId);
        stmt1.bindText(2, regId);
        stmt1.bindText(3, chanId);
        stmt1.bindInt(4, paramCount);
        stmt1.bindInt(5, time(NULL));
        stmt1.bindInt(6, time(NULL));
        if (stmt1.step() != SQLITE_DONE)
        {
            log_e("insert reg failed");
        }
    }

    cJSON_Delete(root);
    log_i("insert table reg success");
    return true;
}

static bool initRegParamTable()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists regParam( \
                    prodect_id text, \
                    reg_id text, \
                    chan_id text, \
                    param_id integer, \
                    param_type text, \
                    sys_start_time integer, \
                    sys_update_time integer, \
                    primary key(prodect_id, reg_id, chan_id, param_id) \
                    )";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table regParam failed");
        return false;
    }
    log_i("create table regParam success");

    ifstream in(REG_PATH);
    if (!in)
    {
        log_e("open reg.json failed");
        return false;
    }

    string regStr((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cJSON *root = cJSON_Parse(regStr.c_str());
    if (!root)
    {
        log_e("cJSON_Parse failed");
        return false;
    }

    cJSON *reg = cJSON_GetObjectItem(root, "reg");
    if (!reg)
    {
        log_e("cJSON_GetObjectItem failed");
        return false;
    }

    int size = cJSON_GetArraySize(reg);
    if (size <= 0)
    {
        log_e("cJSON_GetArraySize failed");
        return false;
    }

    for (int i = 0; i < size; i++)
    {
        cJSON *item = cJSON_GetArrayItem(reg, i);
        if (!item)
        {
            log_e("cJSON_GetArrayItem failed");
            return false;
        }

        cJSON *addr = cJSON_GetObjectItem(item, "addr");
        if (!addr)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        cJSON *chan = cJSON_GetObjectItem(item, "chan");
        if (!chan)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        cJSON *param = cJSON_GetObjectItem(item, "param");
        if (!param)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        int paramSize = cJSON_GetArraySize(param);
        if (paramSize <= 0)
        {
            log_e("cJSON_GetArraySize failed");
            return false;
        }

        for (int j = 0; j < paramSize; j++)
        {
            cJSON *paramItem = cJSON_GetArrayItem(param, j);
            if (!paramItem)
            {
                log_e("cJSON_GetArrayItem failed");
                return false;
            }

            cJSON *param_id = cJSON_GetObjectItem(paramItem, "param_id");
            if (!param_id)
            {
                log_e("cJSON_GetObjectItem failed");
                return false;
            }

            cJSON *param_type = cJSON_GetObjectItem(paramItem, "param_type");
            if (!param_type)
            {
                log_e("cJSON_GetObjectItem failed");
                return false;
            }

            string sqlStr = "insert into regParam(prodect_id, reg_id, chan_id, param_id, param_type, sys_start_time, sys_update_time) \
                            values(?, ?, ?, ?, ?, ?, ?)";
            string prodectId = PRODUCT_ID;
            string regId = addr->valuestring;
            string chanId = chan->valuestring;
            string paramType = param_type->valuestring;
            Stmt stmt1(sql, sqlStr);
            stmt1.bindText(1, prodectId);
            stmt1.bindText(2, regId);
            stmt1.bindText(3, chanId);
            stmt1.bindInt(4, param_id->valueint);
            stmt1.bindText(5, paramType);
            stmt1.bindInt(6, time(NULL));
            stmt1.bindInt(7, time(NULL));
            if (stmt1.step() != SQLITE_DONE)
            {
                log_e("insert regParam failed");
            }
        }
    }

    cJSON_Delete(root);
    log_i("insert table regParam success");
    return true;
}

static bool initPropertyTable()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists property( \
                    prodect_id text, \
                    property_id integer, \
                    operate_mode text, \
                    reg_id integer, \
                    chan_id integer, \
                    param_id integer, \
                    sys_start_time integer, \
                    sys_update_time integer, \
                    primary key(prodect_id, property_id) \
                    )";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table property failed");
        return false;
    }
    log_e("create table property success");
    return true;
}

static bool initEventTable()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists event( \
                    prodect_id text, \
                    event_id text, \
                    reg_id text, \
                    chan_id text, \
                    obj_count integer, \
                    sys_start_time integer, \
                    sys_update_time integer, \
                    primary key(prodect_id, event_id) \
                    )";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table event failed");
        return false;
    }
    log_e("create table event success");

    ifstream in(EVENT_PATH);
    if (!in)
    {
        log_e("open file %s failed", EVENT_PATH);
        return false;
    }

    string inStr((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cJSON *root = cJSON_Parse(inStr.c_str());
    if (!root)
    {
        log_e("cJSON_Parse failed");
        return false;
    }

    cJSON *event = cJSON_GetObjectItem(root, "event");
    if (!event)
    {
        log_e("cJSON_GetObjectItem failed");
        return false;
    }

    int size = cJSON_GetArraySize(event);
    if (size <= 0)
    {
        log_e("cJSON_GetArraySize failed");
        return false;
    }

    for(int i  = 0; i < size; i++)
    {
        cJSON *item = cJSON_GetArrayItem(event, i);
        if (!item)
        {
            log_e("cJSON_GetArrayItem failed");
            return false;
        }
        
        cJSON *event_id = cJSON_GetObjectItem(item, "event_id");
        if (!event_id)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        cJSON *reg_id = cJSON_GetObjectItem(item, "reg_id");
        if (!reg_id)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        cJSON *chan_id = cJSON_GetObjectItem(item, "chan_id");
        if (!chan_id)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        cJSON *obj_count = cJSON_GetObjectItem(item, "object_count");
        if (!obj_count)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        string strSql = "insert into event(prodect_id, \
                        event_id, \
                        reg_id, \
                        chan_id, \
                        obj_count, \
                        sys_start_time, \
                        sys_update_time) \
                        values(?, ?, ?, ?, ?, ?, ?)";
        string prodectId = PRODUCT_ID;
        string eventId = event_id->valuestring;
        string regId = reg_id->valuestring;
        string chanId = chan_id->valuestring;
        
        Stmt stmt1(sql, strSql);
        stmt1.bindText(1, prodectId);
        stmt1.bindText(2, eventId);
        stmt1.bindText(3, regId);
        stmt1.bindText(4, chanId);
        stmt1.bindInt(5, obj_count->valueint);
        stmt1.bindInt(6, time(NULL));
        stmt1.bindInt(7, time(NULL));
        if (stmt1.step() != SQLITE_DONE)
        {
            log_e("insert into event failed");
            return false;
        }
    }
    cJSON_Delete(root);
    log_i("insert into event success");
    return true;
}

static bool initEventParamTable()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists eventParam( \
                    prodect_id text, \
                    event_id text, \
                    object_id text, \
                    object_type text, \
                    param_name text, \
                    param_id integer, \
                    sys_start_time integer, \
                    sys_update_time integer, \
                    primary key(prodect_id, event_id, object_id, param_name) \
                    )";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table eventParam failed");
        return false;
    }
    log_i("create table eventParam success");

    ifstream in(EVENT_PATH);
    if (!in.is_open())
    {
        log_e("open file %s failed", EVENT_PATH);
        return false;
    }

    string inStr((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    cJSON *root = cJSON_Parse(inStr.c_str());
    if (!root)
    {
        log_e("cJSON_Parse failed");
        return false;
    }

    cJSON *event = cJSON_GetObjectItem(root, "event");
    if (!event)
    {
        log_e("cJSON_GetObjectItem failed");
        return false;
    }

    int size = cJSON_GetArraySize(event);
    if (size <= 0)
    {
        log_e("cJSON_GetArraySize failed");
        return false;
    }

    for(int i  = 0; i < size; i++)
    {
        cJSON *item = cJSON_GetArrayItem(event, i);
        if (!item)
        {
            log_e("cJSON_GetArrayItem failed");
            return false;
        }

        cJSON *event_id = cJSON_GetObjectItem(item, "event_id");
        if (!event_id)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        cJSON *param = cJSON_GetObjectItem(item, "param");
        if (!param)
        {
            log_e("cJSON_GetObjectItem failed");
            return false;
        }

        int param_size = cJSON_GetArraySize(param);
        if (param_size <= 0)
        {
            log_e("cJSON_GetArraySize failed");
            return false;
        }

        for(int j = 0; j < param_size; j++)
        {
            cJSON *param_item = cJSON_GetArrayItem(param, j);
            if (!param_item)
            {
                log_e("cJSON_GetArrayItem failed");
                return false;
            }

            cJSON *object_id = cJSON_GetObjectItem(param_item, "object_id");
            if (!object_id)
            {
                log_e("cJSON_GetObjectItem failed");
                return false;
            }

            cJSON *object_type = cJSON_GetObjectItem(param_item, "object_type");
            if (!object_type)
            {
                log_e("cJSON_GetObjectItem failed");
                return false;
            }

            cJSON *param_id = cJSON_GetObjectItem(param_item, "param_id");
            if (!param_id)
            {
                log_e("cJSON_GetObjectItem failed");
                return false;
            }

            cJSON *param_name = cJSON_GetObjectItem(param_item, "param_name");
            if (!param_name)
            {
                log_e("cJSON_GetObjectItem failed");
                return false;
            }

            string paraStr = "insert into eventParam(prodect_id, event_id, object_id, object_type, param_name, param_id, sys_start_time, sys_update_time) \
                                values(?, ?, ?, ?, ?, ?, ?, ?)";
            string prodectId = PRODUCT_ID;
            string eventId = event_id->valuestring;
            string objectId = object_id->valuestring;
            string objectType = object_type->valuestring;
            string paramName = param_name->valuestring;

            Stmt stmt(sql, paraStr);
            stmt.bindText(1, prodectId);
            stmt.bindText(2, eventId);
            stmt.bindText(3, objectId);
            stmt.bindText(4, objectType);
            stmt.bindText(5, paramName);
            stmt.bindInt(6, param_id->valueint);
            stmt.bindInt(7, time(NULL));
            stmt.bindInt(8, time(NULL));
            if (stmt.step() != SQLITE_DONE)
            {
                log_e("insert into eventParam failed");
                return false;
            }
        }
    }

    cJSON_Delete(root);
    log_i("insert into eventParam success");
    return true;
}

static bool initServiceTable()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists service( \
                    prodect_id text, \
                    service_id text, \
                    reg_id integer, \
                    chan_id integer, \
                    sys_start_time integer, \
                    sys_update_time integer, \
                    primary key(prodect_id, service_id) \
                    )";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table service failed");
        return false;
    }
    log_i("create table service success");
    return true;
}

static bool initServiceParamTable()
{
    Sql sql(DB_PATH);
    string str = "create table if not exists serviceParam( \
                    prodect_id text, \
                    service_id text, \
                    object_id text, \
                    param_id integer, \
                    sys_start_time integer, \
                    sys_update_time integer, \
                    primary key(prodect_id, service_id, object_id) \
                    )";
    Stmt stmt(sql, str);
    if (stmt.step() != SQLITE_DONE)
    {
        log_e("create table serviceParam failed");
        return false;
    }
    log_e("create table serviceParam success");
    return true;
}

bool Db::init()
{
    static bool init = false;
    if (init)
    {
        return true;
    }

    // initNetTopTable();
    initRegTable();
    initRegParamTable();
    // initPropertyTable();
    initEventTable();
    initEventParamTable();
    // initServiceTable();
    // initServiceParamTable();

    // Sql sql(dbPath_m);
    // string registDev = "create table if not exists registDev(nid integer primary key, sn text, name text, zid integer, lastTime integer)";
    // Stmt stmt(sql, registDev);
    // if (stmt.step() != SQLITE_DONE)
    // {
    //     log_e("create table registDev failed");
    //     return false;
    // }
    // log_e("new Sql success");
    init = true;
    return true;
}

bool Db::registDev(uint32_t nid, uint16_t zid, const string &sn)
{
    Sql sql(dbPath_m);
    string name = getDevName(sn);
    if (name.empty())
    {
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
    if (stmt.step() != SQLITE_DONE)
    {
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
    if (stmt.step() != SQLITE_DONE)
    {
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
    if (stmt.step() != SQLITE_DONE)
    {
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
    if (stmt.step() == SQLITE_ROW)
    {
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
    if (stmt.step() != SQLITE_DONE)
    {
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
    if (stmt.step() != SQLITE_DONE)
    {
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
    if (stmt.step() == SQLITE_ROW)
    {
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
    string sqlStr = "select nid from netTopo where nid = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindInt(1, nid);
    if (stmt.step() == SQLITE_ROW)
    {
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
    if (stmt.step() == SQLITE_ROW)
    {
        return true;
    }

    return false;
}

uint32_t Db::getDevCount()
{
    Sql sql(dbPath_m);
    string sqlStr = "select count(*) from dev";
    Stmt stmt(sql, sqlStr);
    if (stmt.step() == SQLITE_ROW)
    {
        return stmt.getInt(0);
    }

    return 0;
}

bool Db::randomNid(uint32_t &nid)
{
    Sql sql(dbPath_m);
    string sqlStr = "select nid from nidlist";
    Stmt stmt(sql, sqlStr);
    if (stmt.step() != SQLITE_ROW)
    {

        return false;
    }

    nid = stmt.getInt(0);
    string sqlStr1 = "delete from nidlist where nid = ?";
    Stmt stmt1(sql, sqlStr1);
    stmt1.bindInt(1, nid);
    if (stmt1.step() != SQLITE_DONE)
    {
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
    if (stmt.step() == SQLITE_ROW)
    {
        return false;
    }
    zid = stmt.getInt(0);
    string sqlStr1 = "delete from zidlist where zid = ?";
    Stmt stmt1(sql, sqlStr1);
    stmt1.bindInt(1, zid);
    if (stmt1.step() != SQLITE_DONE)
    {
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
    if (stmt.step() == SQLITE_ROW)
    {
        return stmt.getText(0);
    }

    return "";
}

bool Db::isRegLegal(const string &pd, const string &reg, const string &chan)
{
    Sql sql(dbPath_m);
    string sqlStr = "select prodect_id from reg where prodect_id = ? and reg_id = ? and chan_id = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, pd);
    stmt.bindText(2, reg);
    stmt.bindText(3, chan);
    if (stmt.step() == SQLITE_ROW)
    {
        return true;
    }

    return false;
}

uint8_t Db::getRegParamCount(const string &pd, const string &reg, const string &chan)
{
    Sql sql(dbPath_m);
    string sqlStr = "select param_count from reg where prodect_id = ? and reg_id = ? and chan_id = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, pd);
    stmt.bindText(2, reg);
    stmt.bindText(3, chan);
    if (stmt.step() == SQLITE_ROW)
    {
        return stmt.getInt(0);
    }

    return 0;
}

string Db::getRegParamType(const string &pd, const string &reg, const string &chan, uint8_t index)
{
    Sql sql(dbPath_m);
    string sqlStr = "select param_type from reg where prodect_id = ? and reg_id = ? and chan_id = ? and param_id = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, pd);
    stmt.bindText(2, reg);
    stmt.bindText(3, chan);
    stmt.bindInt(4, index);
    if (stmt.step() == SQLITE_ROW)
    {
        return stmt.getText(0);
    }

    return "";
}

bool Db::isRegInEvent(const string &pd, const string &reg, const string &chan)
{
    Sql sql(dbPath_m);
    string sqlStr = "select event_id from event where prodect_id = ? and reg_id = ? and chan_id = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, pd);
    stmt.bindText(2, reg);
    stmt.bindText(3, chan);
    if (stmt.step() == SQLITE_ROW)
    {
        return true;
    }

    return false;
}

uint8_t Db::getEventObjCount(const string &pd, const string &eventId)
{
    Sql sql(dbPath_m);
    string sqlStr = "select obj_count from event where prodect_id = ? and event_id = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, pd);
    stmt.bindText(2, eventId);
    if (stmt.step() == SQLITE_ROW)
    {
        return stmt.getInt(0);
    }

    return 0;
}

uint8_t Db::getEventCount(const string &pd, const string &reg, const string &chan)
{
    uint8_t count = 0;
    Sql sql(dbPath_m);
    string sqlStr = "select event_id from event where prodect_id = ? and reg_id = ? and chan_id = ?";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, pd);
    stmt.bindText(2, reg);
    stmt.bindText(3, chan);
    for (uint8_t i = 0; stmt.step() == SQLITE_ROW; i++)
    {
        count++;
    }

    return count;
}

string Db::getEventId(const string &pd, const string &reg, const string &chan, uint8_t index)
{
    Sql sql(dbPath_m);
    string sqlStr = "select event_id from event where prodect_id = ? and reg_id = ? and chan_id = ? \
                     order by event_id asc";
    Stmt stmt(sql, sqlStr);
    stmt.bindText(1, pd);
    stmt.bindText(2, reg);
    stmt.bindText(3, chan);
    for (uint8_t i = 0; stmt.step() == SQLITE_ROW; i++)
    {
        if (i == index)
        {
            return stmt.getText(0);
        }
    }

    return "";
}

/* ----- End of file ----- */
