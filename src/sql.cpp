#include "sql.h"

Sql::Sql(const string &dbPath)
{
    int ret = sqlite3_open(dbPath.c_str(), &db_m);
    if(ret != SQLITE_OK){
        log_e("sqlite3_open failed, ret = %d", ret);
    }
}

Sql::~Sql()
{
    sqlite3_close(db_m);
}

Stmt::Stmt(Sql &sql, const string &sqlStr):stmt_m(NULL)
{
    int ret = sqlite3_prepare_v2(sql.db_m, sqlStr.c_str(), -1, &stmt_m, NULL);
    if(ret != SQLITE_OK){
        log_e("sqlite3_prepare_v2 failed, ret = %d", ret);
    }
}

Stmt::~Stmt()
{
    sqlite3_reset(stmt_m);
    sqlite3_finalize(stmt_m);
}

int Stmt::bindInt(int index, int value)
{
    int ret = sqlite3_bind_int(stmt_m, index, value);
    if(ret != SQLITE_OK){
        log_e("sqlite3_bind_int failed, ret = %d", ret);
    }
    return ret;
}

int Stmt::bindText(int index, const string &value)
{
    int ret = sqlite3_bind_text(stmt_m, index, value.c_str(), value.size(), SQLITE_STATIC);
    if(ret != SQLITE_OK){
        log_e("sqlite3_bind_text failed, ret = %d", ret);
    }
    return ret;
}

int Stmt::bindBlob(int index, const vector<uint8_t> &value)
{
    int ret = sqlite3_bind_blob(stmt_m, index, value.data(), value.size(), SQLITE_STATIC);
    if(ret != SQLITE_OK){
        log_e("sqlite3_bind_blob failed, ret = %d", ret);
    }
    return ret;
}

int Stmt::bindNull(int index)
{
    int ret = sqlite3_bind_null(stmt_m, index);
    if(ret != SQLITE_OK){
        log_e("sqlite3_bind_null failed, ret = %d", ret);
    }
    return ret;
}

int Stmt::step()
{
    int ret = sqlite3_step(stmt_m);
    if(ret != SQLITE_ROW && ret != SQLITE_DONE){
        log_e("sqlite3_step failed, ret = %d", ret);
    }
    return ret;
}

int Stmt::reset()
{
    int ret = sqlite3_reset(stmt_m);
    if(ret != SQLITE_OK){
        log_e("sqlite3_reset failed, ret = %d", ret);
    }
    return ret;
}

int Stmt::clear()
{
    int ret = sqlite3_clear_bindings(stmt_m);
    if(ret != SQLITE_OK){
        log_e("sqlite3_clear_bindings failed, ret = %d", ret);
    }
    return ret;
}

int Stmt::getInt(int index)
{
    return sqlite3_column_int(stmt_m, index);
}

string Stmt::getText(int index)
{
    const char *text = (const char *)sqlite3_column_text(stmt_m, index);
    return string(text);

}
vector<uint8_t> Stmt::getBlob(int index)
{
    const uint8_t *blob = (const uint8_t *)sqlite3_column_blob(stmt_m, index);
    int size = sqlite3_column_bytes(stmt_m, index);
    return vector<uint8_t>(blob, blob + size);
}

int Stmt::getType(int index)
{
    return sqlite3_column_type(stmt_m, index);
}


/* ----- End of file ----- */
