#pragma once
#include "sqlite3.h"
#include "log.h"
#include "tools.h"
#include <string>
#include <vector>

using namespace std;

//sqlite3 
class Sql{
    friend class Stmt;
public:
    Sql(const string &dbPath);
    ~Sql();

private:
    sqlite3 *db_m;
};

class Stmt{
public:
    Stmt(Sql &sql, const string &sqlStr);
    ~Stmt();

    int bindInt(int index, int value);
    int bindText(int index, const string &value);
    int bindBlob(int index, const vector<uint8_t> &value);
    int bindNull(int index);

    int step();
    int reset();
    int clear();

    int getInt(int index);
    string getText(int index);
    vector<uint8_t> getBlob(int index);
    int getType(int index);

private:
    sqlite3_stmt *stmt_m;
};


/* ----- End of file ----- */
