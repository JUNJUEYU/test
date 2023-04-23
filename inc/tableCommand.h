#pragma once 
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdint.h>
#include "paramBase.h"
using namespace std;



class Command
{
    public:
        string name_m;
        string rw_m;
        uint32_t regAddr_m;
        vector<ParamBase> param_m;
};


class CommandTable
{
    public:
        static CommandTable& getInstance();
        void addCommand(Command cmd);
        bool getCommand(string name, Command &cmd);
        bool getCommand(uint32_t regAddr, Command &cmd);
        int getCommandSize();
    private:
        CommandTable();
        CommandTable(const CommandTable&);
        CommandTable& operator=(const CommandTable&);
        vector<Command> cmdList_m;
};