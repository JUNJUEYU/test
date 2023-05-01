#include "zigbee.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "tools.h"
#define FILE_PATH "./test.json"

// void DataBaseList::addData(DataBase data)
// {
//     this->data_m.push_back(data);
// }

// bool DataBaseList::getData(int num, DataBase &data)
// {
//     for (vector<DataBase>::iterator it = data_m.begin(); it != data_m.end(); it++)
//     {
//         if (it->getNum() == num)
//         {
//             data = *it;
//             return true;
//         }
//     }
//     return false;
// }

// int DataBaseList::getDataSize()
// {
//     return data_m.size();
// }

// void ZigRegTable::addReg(ZigReg reg)
// {
//     this->reg_m.push_back(reg);
// }

// bool ZigRegTable::getReg(int regAddr, ZigReg &reg)
// {
//     for (vector<ZigReg>::iterator it = reg_m.begin(); it != reg_m.end(); it++)
//     {
//         if (it->getRegAddr() == regAddr)
//         {
//             reg = *it;
//             return true;
//         }
//     }
//     return false;
// }

// ZigRegTable& ZigRegTable::getInstance()
// {
//     static ZigRegTable instance;
//     return instance;
// }

// int ZigRegTable::getTableSize()
// {
//     return reg_m.size();
// }

// ZigRegTable::ZigRegTable()
// {
//     char * file = NULL;
//     FILE *fp = fopen(FILE_PATH, "r");
//     if (NULL == fp)
//     {
//         log_i("open file error");
//         return ;
//     }
//     fseek(fp, 0, SEEK_END);
//     long len = ftell(fp);
//     fseek(fp, 0, SEEK_SET);
//     file = (char *)malloc(len + 1);
//     fread(file, 1, len, fp);
//     file[len] = '\0';
//     fclose(fp);
//     cJSON *root = cJSON_Parse(file);
//     if (NULL == root)
//     {
//         log_i("json parse error");
//         return ;
//     }
//     int size = cJSON_GetArraySize(root);
//     for (size_t i = 0; i < size; i++)
//     {
//         int reg;
//         cJSON *item = cJSON_GetArrayItem(root, i);
//         if (NULL != item)
//         {
//             reg = Trans::toHexInt(string(item->string));
//             ZigReg zigReg(reg);
//             int count = cJSON_GetArraySize(item);
//             for (size_t j = 0; j < count; j++)
//             {
//                 cJSON *item1 = cJSON_GetArrayItem(item, j);
//                 if (NULL != item1)
//                 {
//                     cJSON *num = cJSON_GetObjectItem(item1, "num");
//                     if (NULL == num)
//                     {
//                         log_i("num is null");
//                         continue;
//                     }
//                     cJSON *type = cJSON_GetObjectItem(item1, "type");
//                     if (NULL == type)
//                     {
//                         log_i("type is null");
//                         continue;
//                     }

//                     ZigData zigData(num->valueint, type->valuestring);
//                     zigReg.addData(zigData);
//                 }
//             }
//             for(int i = 0; i < zigReg.getDataSize(); i++){
//                 ZigData zigData = zigReg.getData(i);
//                 log_i("reg = %06x, num = %d, type = %s", zigReg.getRegAddr(),zigData.num_m, zigData.type_m.c_str());
//             }
//             this->addReg(zigReg);
//         }
//     }    
//     cJSON_Delete(root);
//     free(file);
// }





/* ----- End of file ----- */
