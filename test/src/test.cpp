#include "msgQueue.h"
#include "subDevManager.h"
#include "pluginAPI.h"
#include "zigProtStack.h"
#include "tools.h"
#include "log.h"
#include <stdlib.h>
#define ZID 0x8888

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


vector<uint8_t> testNetAccDynamic()
{
    vector<uint8_t> src;
    /* Net layer */
    /* head */
    src.push_back(0xAA);
    src.push_back(0xD1);
    /* zigbee id */
    src.push_back(0x00);
    src.push_back(0x01);

    /* Trans layer */
    /* length & sum */
    src.push_back(36);
    src.push_back(0x00);
    /* Protocol version */
    src.push_back(1);
    src.push_back(0x00);
    /* Target net code */
    src.push_back(1);
    src.push_back(0x00);
    src.push_back(0x00);
    src.push_back(0x00);
    /* Source net code */
    src.push_back(0x00);
    src.push_back(0x00);
    src.push_back(0x00);
    src.push_back(0x00);

    /* App layer */
    /* Msg type */
    src.push_back(1);
    src.push_back(1);
    /* Msg id */
    src.push_back(0x55);
    src.push_back(0x66);

    /* Data */
    src.push_back('1');
    src.push_back('2');
    src.push_back('3');
    src.push_back('4');
    src.push_back('5');
    src.push_back('6');
    src.push_back('7');
    src.push_back('8');
    src.push_back('9');
    src.push_back('0');
    src.push_back('1');
    src.push_back('2');
    src.push_back('3');
    src.push_back('4');
    src[4] = src.size() - 4;

    uint8_t sum = 0;
    for(vector<uint8_t>::iterator it = src.begin() + 6; it != src.end(); it++){
        sum += *it;
    }

    src[5] = sum;
    src.push_back(0x55);

    return src;
}

vector<uint8_t> testNetAccStatic()
{
    vector<uint8_t> src;
    /* Net layer */
    /* head */
    src.push_back(0xAA);
    src.push_back(0xD1);
    /* zigbee id */
    src.push_back(0x00);
    src.push_back(0x01);

    /* Trans layer */
    /* length & sum */
    src.push_back(38);
    src.push_back(0x00);
    /* Protocol version */
    src.push_back(1);
    src.push_back(0x00);
    /* Target net code */
    src.push_back(1);
    src.push_back(0x00);
    src.push_back(0x00);
    src.push_back(0x00);
    /* Source net code */
    src.push_back(0x00);
    src.push_back(0x00);
    src.push_back(0x00);
    src.push_back(0x00);

    /* App layer */
    /* Msg type */
    src.push_back(1);
    src.push_back(2);
    /* Msg id */
    src.push_back(0x66);
    src.push_back(0x77);

    /* Data */
    src.push_back('1');
    src.push_back('2');
    src.push_back('3');
    src.push_back('4');
    src.push_back('5');
    src.push_back('6');
    src.push_back('7');
    src.push_back('8');
    src.push_back('9');
    src.push_back('0');
    src.push_back('1');
    src.push_back('2');
    src.push_back('3');
    src.push_back('4');
    src.insert(src.end(), 6, 0);
    src.push_back((uint8_t)(ZID >> 8));
    src.push_back((uint8_t)ZID);
    src[4] = src.size() - 4;

    uint8_t sum = 0;
    for(vector<uint8_t>::iterator it = src.begin() + 6; it != src.end(); it++){
        sum += *it;
    }

    src[5] = sum;
    src.push_back(0x55);

    return src;
}

vector<uint8_t> testHeart()
{
    vector<uint8_t> src;
    /* Net layer */
    /* head */
    src.push_back(0xAA);
    src.push_back(0xD1);
    /* zigbee id */
    src.push_back((uint8_t)(ZID >> 8));
    src.push_back((uint8_t)ZID);

    /* Trans layer */
    /* length & sum */
    src.push_back(0);
    src.push_back(0x00);
    /* Protocol version */
    src.push_back(1);
    src.push_back(0x00);
    /* Target net code */
    src.push_back(1);
    src.push_back(0x00);
    src.push_back(0x00);
    src.push_back(0x00);
    /* Source net code */
    src.push_back(0x30);
    src.push_back(0x30);
    src.push_back(0x30);
    src.push_back(0x30);

    /* App layer */
    /* Msg type */
    src.push_back(2);
    src.push_back(1);
    /* Msg id */
    src.push_back(0x22);
    src.push_back(0x33);

    /* Data */
    src.push_back(1);
    src.push_back(1);
    src.push_back(1);
    src.push_back(1);
    src.push_back(0);
    src[4] = src.size() - 4;

    uint8_t sum = 0;
    for(vector<uint8_t>::iterator it = src.begin() + 6; it != src.end(); it++){
        sum += *it;
    }

    src[5] = sum;
    src.push_back(0x55);

    return src;
}

void packNetLayer(vector<uint8_t> &src, uint16_t zigbeeId)
{
    vector<uint8_t> head;
    /* head */
    head.push_back(0xAA);
    head.push_back(0xD1);
    /* zigbee id */
    head.push_back(zigbeeId >> 8);
    head.push_back(zigbeeId & 0xFF);
    src.insert(src.begin(), head.begin(), head.end());
    src.push_back(0x55);
}

void packTransLayer(vector<uint8_t> &src, uint32_t srcNid)
{
    vector<uint8_t> head;
    head.push_back(src.size() + 12);
    head.push_back(0x00);
    head.push_back(1);
    head.push_back(0x00);
    head.push_back(1);
    head.insert(head.end(), 3, 0);
    head.push_back((uint8_t)srcNid);
    head.push_back((uint8_t)(srcNid >> 8));
    head.push_back((uint8_t)(srcNid >> 16));
    head.push_back((uint8_t)(srcNid >> 24));
    head.insert(head.end(), src.begin(), src.end());
    uint8_t sum = 0;
    for(vector<uint8_t>::iterator it = head.begin() + 2; it != head.end(); it++){
        sum += *it;
    }
    head[1] = sum;

    src.clear();
    src.insert(src.end(), head.begin(), head.end());
}

vector<uint8_t> testSigUp()
{
    vector<uint8_t> msg;
    msg.push_back(2);
    msg.push_back(2);
    msg.push_back(0x33);
    msg.push_back(0x33);
    msg.insert(msg.end(), 4, 0);

    for(int i = 0; i < 4; i++){
        msg.push_back(1);
        msg.push_back(1);

        msg.push_back(1);
        msg.push_back(100);
        msg.insert(msg.end(), 3, 0);
        msg.push_back(200);
        msg.insert(msg.end(), 3, 0);
    }

    float value1 = 10.8;
    float value2 = 20.8;
    msg.push_back(*(uint8_t *)&value1);
    msg.push_back(*((uint8_t *)&value1 + 1));
    msg.push_back(*((uint8_t *)&value1 + 2));
    msg.push_back(*((uint8_t *)&value1 + 3));
    msg.push_back(*(uint8_t *)&value2);
    msg.push_back(*((uint8_t *)&value2 + 1));
    msg.push_back(*((uint8_t *)&value2 + 2));
    msg.push_back(*((uint8_t *)&value2 + 3));
    packTransLayer(msg, 0x30303030);
    packNetLayer(msg, ZID);

    return msg;
}
/* ----- End of file ----- */
