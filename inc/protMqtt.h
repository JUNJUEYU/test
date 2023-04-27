#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;


class MqttProt{
    public:
        string uuid(){return uuid_m;};
        string tarPd(){return tarPd_m;};
        string tarSn(){return tarSn_m;};
    protected:
        string uuid_m;
        string tarPd_m;
        string tarSn_m;
};

class MqttProtProSet:public MqttProt
{
    public:
        bool parse(const string &msg);
        int paramSize();
        string proName();
        string paramName(int index);

    private:
        string proName_m;
        map<string, string> param_m;
};