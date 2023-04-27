#include "descTblRegister.h"
#include "json.h"
#include "log.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>


RegisterDescTbl::RegisterDescTbl(const string &path)
{
    char * file = NULL;
    FILE *fp = fopen(path.c_str(), "r");
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

    Json json(file);
    cJSON *root = json.root();
    if (NULL == root){
        log_i("json parse error");
        return ;
    }

    cJSON *reg = cJSON_GetObjectItem(root, "register");
    if(NULL == reg){
        return ;
    }
    int size = cJSON_GetArraySize(reg);
    for(int i = 0; i < size; i++){
        RegisterDesc regDesc;
        cJSON *item = cJSON_GetArrayItem(reg, i);
        cJSON *regAddr = cJSON_GetObjectItem(item, "addr");
        if(NULL == regAddr){
            continue;
        }
        regDesc.regAddr_m = Trans::toHexInt(regAddr->valuestring);

        cJSON *rw = cJSON_GetObjectItem(item, "rw");
        if(NULL == rw){
            continue;
        }
        regDesc.rw_m = rw->valuestring;

        cJSON *property = cJSON_GetObjectItem(item, "property");
        if(NULL == property){
            continue;
        }
        int proSize = cJSON_GetArraySize(property);
        for(int index = 0; index < proSize; index++){
            cJSON *proItem = cJSON_GetArrayItem(property, index);
            if(NULL == proItem){
                continue;
            }

            Property pro;
            cJSON *name = cJSON_GetObjectItem(proItem, "name");
            if(NULL == name){
                continue;
            }
            pro.name_m = name->valuestring;

            cJSON *offset = cJSON_GetObjectItem(proItem, "offset");
            if(NULL == offset){
                continue;
            }
            pro.offest_m = offset->valueint;

            cJSON *dataType = cJSON_GetObjectItem(proItem, "dataType");
            if(NULL == dataType){
                continue;
            }
            pro.dataType_m = dataType->valuestring;

            regDesc.proList_m.push_back(pro);
        }
        regList_m.push_back(regDesc);
    }
    free(file);
}

int RegisterDescTbl::getRegDescSize()
{
    return regList_m.size();
}

bool RegisterDescTbl::getRegDesc(uint16_t regAddr, RegisterDesc &regDesc)
{
    for(int i = 0; i < regList_m.size(); i++){
        if(regList_m[i].regAddr_m == regAddr){
            regDesc = regList_m[i];
            return true;
        }
    }
    return false;
}