#include "msgQueue.h"
#include "subDevManager.h"
#include "pluginAPI.h"
#include "protocolStack.h"
#include "tools.h"
#include "log.h"
#include "threadComRead.h"
#include <stdlib.h>

class API:public ProtAPI{
    public:
        API(){cout << "I am api"<<endl;};
        ~API(){cout << " api bye"<<endl;}
        bool trans(const vector<uint8_t> &, string &){};
        bool trans(const string &, vector<uint8_t> &){};
        string getTopic(){cout << "api topic"<<endl;};
};

 
static vector<uint8_t> packAppLayerTest()
{
    string sn = "123456789";
    vector<uint8_t> data;
        /* App layer */
    data.push_back(0x01);
    data.push_back(0x01);
    data.push_back(0x0);
    data.push_back(0x01);
    data.insert(data.end(), sn.begin(), sn.end());
    Log::print("App layer data: ", data);
    return data;
}

static vector<uint8_t> packTransLayerTest(vector<uint8_t> src)
{
    vector<uint8_t> data;

    data.push_back(12 + src.size());
    data.push_back(0x0);
    data.push_back(0x1);
    data.push_back(0x0);
    for(int i = 0; i < 8; i++)
        data.push_back(0x0);
    data.insert(data.end(), src.begin(), src.end());
    Sum sum(vector<uint8_t>(data.begin() + 2, data.end()));
    data[1] = sum.getSum();
    Log::print("Trans layer data: ", data);
    return data;
}

static vector<uint8_t> packNetLayerTest(vector<uint8_t> src)
{
    vector<uint8_t> data;

    data.push_back(0xAA);
    data.push_back(0xD1);
    data.push_back(0x0);
    data.push_back(0x1);
    data.insert(data.end(), src.begin(), src.end());
    data.push_back(0x55);
    Log::print("Net layer data: ", data);

    return data;
}


vector<uint8_t> packDataTest()
{
    vector<uint8_t> data;
    data = packAppLayerTest();
    data = packTransLayerTest(data);
    data = packNetLayerTest(data);

    return data;
}

/* ----- End of file ----- */
