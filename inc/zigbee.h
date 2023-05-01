#include <vector>
#pragma once
#include <stdint.h>
#include <string>
using namespace std;

#pragma pack(1)
typedef struct{
    uint16_t head;
    uint16_t zid;
}ZigMsgNetLayer_t;

typedef struct{
    uint8_t len;
    uint8_t sum;
    uint16_t ver;
    uint32_t tarNid;
    uint32_t srcNid;
}ZigMsgTransLayer_t;

typedef struct{
    uint8_t cmd;
    uint8_t reg;
    uint8_t chan;
    uint8_t msgid;
}ZigMsgAppLayer_t;

typedef struct{
    ZigMsgNetLayer_t netLayer;
    ZigMsgTransLayer_t transLayer;
    ZigMsgAppLayer_t appLayer;
    uint8_t data[0];
}ZigMsg_t;
#pragma pack()


// class DataBase{
//     public:
//         DataBase(uint8_t num, string type):num_m(num), type_m(type){};
//         uint8_t getNum(){return num_m;};
//     public:
//         uint8_t uint8;
//         uint16_t uint16;
//         uint32_t uint32;
//         float float32;
//         string str;
//     private:
//         uint8_t num_m;
//         string type_m;
// };

// class DataBaseList{
//     public:
//         DataBaseList(){};
//         void addData(DataBase data);

//         bool getData(int num, DataBase &data);
//         int getDataSize();

//     private:
//         vector<DataBase> data_m;
// };

// class ZigData{
//     public:
//         ZigData(int num, string type):num_m(num), type_m(type){};

//     public:
//         int num_m;
//         string type_m;
// };

// class ZigReg{
//     public:
//         ZigReg(){};
//         ZigReg(int regAddr):regAddr_m(regAddr){};
//         void addData(ZigData data){
//             this->data.push_back(data);
//         };

//         ZigData getData(int num){
//             return data[num];
//         };

//         int getDataSize(){
//             return data.size();
//         };
        
//         int getRegAddr(){
//             return regAddr_m;
//         };

//     private:
//         int regAddr_m;
//         vector<ZigData> data;
// };

// class ZigRegTable{
//     public:
//         static ZigRegTable& getInstance();
//         void addReg(ZigReg reg);
//         bool getReg(int regAddr, ZigReg &reg);
//         int getTableSize();

//     private:
//         ZigRegTable &operator=(const ZigRegTable &){};
//         ZigRegTable(const ZigRegTable &){};
//         ZigRegTable();
//         ~ZigRegTable(){};

//         vector<ZigReg> reg_m;
// };

/* ----- End of file ----- */
