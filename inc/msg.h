#pragma once
#include <vector>
#include <string>
#include <stdint.h>
using namespace std;

class Msg{
    public:
        Msg():isLegal_m(false){};
        virtual ~Msg(){};
        virtual bool isLegal(){return isLegal_m;};
        virtual string getStrMsg(){return string();};
        virtual vector<uint8_t> getHexMsg(){return vector<uint8_t>();};
    protected:
        Msg(bool isLegal):isLegal_m(isLegal){};
        bool isLegal_m;
};


class MsgNet:public Msg{
    public:
        MsgNet(const string &str):msg_m(str){};
        MsgNet(bool isLegal):Msg(isLegal){};
        ~MsgNet(){};
        virtual string getStrMsg(){
            return msg_m;
        };
    
    private:
        string msg_m;
};


class MsgDev:public Msg{
    public:
        MsgDev(const vector<uint8_t> &data):msg_m(data){};
        MsgDev(bool isLegal):Msg(isLegal){};
        ~MsgDev(){};
        virtual vector<uint8_t> getHexMsg(){
            return msg_m;
        };
    
    private:
        vector<uint8_t> msg_m;
};


/* ----- End of file ----- */
