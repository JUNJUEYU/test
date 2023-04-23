#include "tableProperty.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "tools.h"

#define FILE_PATH "./command.json"

PropertyTable::PropertyTable()
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

    cJSON *property = cJSON_GetObjectItem(root, "property");

    int size = cJSON_GetArraySize(property);
    for (int i = 0; i < size; i++){
        cJSON *item = cJSON_GetArrayItem(property, i);
        if(NULL == item){
            continue;
        }

        Property pro;

        cJSON *reg = cJSON_GetObjectItem(item, "reg");
        if (NULL == reg){
            continue;
        }
        pro.regAddr_m = Trans::toHexInt(reg->valuestring);

        cJSON *name = cJSON_GetObjectItem(item, "name");
        if (NULL == name){
            continue;
        }
        pro.name_m = name->valuestring;

        cJSON *rw = cJSON_GetObjectItem(item, "rw");
        if (NULL == rw){
            continue;
        }
        pro.rw_m = rw->valuestring;

        cJSON *param = cJSON_GetObjectItem(item, "param");
        if (NULL == param){
            continue;
        }

        int paramSize = cJSON_GetArraySize(param);
        for (int j = 0; j < paramSize; j++){
            ParamBase paramBase;
            cJSON *paramItem = cJSON_GetArrayItem(param, j);
            if(NULL == paramItem){
                continue;
            }

            cJSON *paramName = cJSON_GetObjectItem(paramItem, "name");
            if (NULL != paramName){
                paramBase.name = paramName->valuestring;
            }

            cJSON *num = cJSON_GetObjectItem(paramItem, "num");
            if (NULL != num){
                paramBase.num = num->valueint;
            }
            paramBase.num = num->valueint;

            cJSON *type = cJSON_GetObjectItem(paramItem, "type");
            if(NULL == type){
                continue;
            }
            paramBase.type = type->valuestring;
            
            pro.param_m.push_back(paramBase);
        } 
        proList_m.push_back(pro);
    }
    cJSON_Delete(root);
    free(file);
}

PropertyTable & PropertyTable::getInstance()
{
    static PropertyTable instance;
    return instance;
}


bool PropertyTable::getProperty(uint32_t regAddr, Property &cmd)
{
    for (vector<Property>::iterator it = proList_m.begin(); it != proList_m.end(); it++)
    {
        if (it->regAddr_m == regAddr)
        {
            cmd = *it;
            return true;
        }
    }
}

bool PropertyTable::getProperty(string name, Property &cmd)
{
    for (vector<Property>::iterator it = proList_m.begin(); it != proList_m.end(); it++)
    {
        if (it->name_m == name)
        {
            cmd = *it;
            return true;
        }
    }
}


