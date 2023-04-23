#include "tableCommand.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "tools.h"

#define FILE_PATH "./command.json"


CommandTable::CommandTable()
{
    char * file = NULL;
    FILE *fp = fopen(FILE_PATH, "r");
    if (NULL == fp)
    {
        log_i("open file error");
        return ;
    }
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    file = (char *)malloc(len + 1);
    fread(file, 1, len, fp);
    file[len] = '\0';
    fclose(fp);
    cJSON *root = cJSON_Parse(file);
    if (NULL == root)
    {
        log_i("json parse error");
        return ;
    }

    cJSON *command = cJSON_GetObjectItem(root, "command");

    int size = cJSON_GetArraySize(command);
    for (int i = 0; i < size; i++){
        cJSON *item = cJSON_GetArrayItem(command, i);
        if(NULL == item){
            continue;
        }

        Command cmd;

        cJSON *reg = cJSON_GetObjectItem(item, "reg");
        if(NULL == reg){
            continue;
        }
        cmd.regAddr_m = Trans::toHexInt(reg->valuestring);

        cJSON *name = cJSON_GetObjectItem(item, "name");
        if(NULL == name){
            continue;
        }
        cmd.name_m = name->valuestring;

        cJSON *param = cJSON_GetObjectItem(item, "param");
        if(NULL == param){
            continue;
        }
        int paramSize = cJSON_GetArraySize(param);
        for (int j = 0; j < paramSize; j++){
            cJSON *item1 = cJSON_GetArrayItem(param, j);
            if(NULL == item1){
                continue;
            }
            ParamBase paramBase;
            cJSON *name = cJSON_GetObjectItem(item1, "name");
            if(NULL != name){
                paramBase.name = name->valuestring;
            }

            cJSON *num = cJSON_GetObjectItem(item1, "num");
            if(NULL == num){
                continue;
            }
            paramBase.num = num->valueint;

            cJSON *type = cJSON_GetObjectItem(item1, "type");
            if(NULL == type){
                continue;
            }
            paramBase.type = type->valuestring;

            cmd.param_m.push_back(paramBase);
        }
        cmdList_m.push_back(cmd);
    }

    cJSON_Delete(root);
    free(file);
}



CommandTable & CommandTable::getInstance()
{
    static CommandTable instance;
    return instance;
}

bool CommandTable::getCommand(uint32_t regAddr, Command & cmd)
{
    for (vector<Command>::iterator it = cmdList_m.begin(); it != cmdList_m.end(); it++){
        if(it->regAddr_m == regAddr){
            cmd = *it;
            return true;
        }
    }

    return false;
}

bool CommandTable::getCommand(string name, Command & cmd)
{
    for (vector<Command>::iterator it = cmdList_m.begin(); it != cmdList_m.end(); it++){
        if(it->name_m == name){
            cmd = *it;
            return true;
        }
    }

    return false;
}