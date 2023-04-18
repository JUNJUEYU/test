#include "zigProtStack.h"
#include "subDevManager.h"
#include "tools.h"
#include "log.h"
#include <algorithm>
#define GW_ZID 0x1000
#define GW_NID 0x00000001

ZigNetLayer::ZigNetLayer(const vector<uint8_t> &data) : rawData_m(data)
{
}

ZigNetLayer::ZigNetLayer(const vector<uint8_t> &data, uint16_t zid) : rawData_m(data), zid_m(zid)
{
    outData_m.push_back(0xAA);
    outData_m.push_back(0xD1);
    outData_m.push_back(zid_m >> 8);
    outData_m.push_back(zid_m & 0xff);
    outData_m.insert(outData_m.end(), rawData_m.begin(), rawData_m.end());
    outData_m.push_back(0x55);
}

bool ZigNetLayer::unpack()
{
    if (0xAA != rawData_m[0] ||
        0xD1 != rawData_m[1])
    {
        return false;
    }

    int rawDataLen = rawData_m.size();
    if (0x55 != rawData_m[rawDataLen - 1])
    {
        return false;
    }

    zid_m = (rawData_m[2] << 8) | rawData_m[3];
    log_d("ZigNetLayer::unpack: zid = %04x", zid_m);

    outData_m = vector<uint8_t>(rawData_m.begin() + 4, rawData_m.end() - 1);
    return true;
}

vector<uint8_t> ZigNetLayer::getData()
{
    return outData_m;
}

uint16_t ZigNetLayer::getZid()
{
    return zid_m;
}

ZigTransLayer::ZigTransLayer(const vector<uint8_t> &data) : rawData_m(data)
{
}

ZigTransLayer::ZigTransLayer(const vector<uint8_t> &data,
                             uint16_t ver,
                             uint32_t tarId,
                             uint32_t srcId)
{
    /* Msg len */
    outData_m.push_back(12 + data.size());
    /* Msg sum */
    outData_m.push_back(0x00);
    /* Protocol version */
    outData_m.push_back(ver & 0xff);
    outData_m.push_back((ver >> 8) & 0xff);
    /* Source nid */
    outData_m.push_back(srcId & 0xff);
    outData_m.push_back((srcId >> 8) & 0xff);
    outData_m.push_back((srcId >> 16) & 0xff);
    outData_m.push_back((srcId >> 24) & 0xff);
    /* Target nid */
    outData_m.push_back(tarId & 0xff);
    outData_m.push_back((tarId >> 8) & 0xff);
    outData_m.push_back((tarId >> 16) & 0xff);
    outData_m.push_back((tarId >> 24) & 0xff);
    /* Data */
    outData_m.insert(outData_m.end(), data.begin(), data.end());
    /* Check sum and put it in */
    outData_m[1] = Sum(vector<uint8_t>(outData_m.begin() + 2, outData_m.end())).getSum();
}

vector<uint8_t> ZigTransLayer::getData()
{
    return outData_m;
}

uint32_t ZigTransLayer::getSrcNid()
{
    return srcNid_m;
}

uint32_t ZigTransLayer::getTarNid()
{
    return tarNid_m;
}

uint16_t ZigTransLayer::getProtVer()
{
    return protVer_m;
}

bool ZigTransLayer::unpack()
{
    if (!integrity())
    {
        return false;
    }

    protVer_m = rawData_m[3] << 8 | rawData_m[2];
    tarNid_m = rawData_m[7] << 24 | rawData_m[6] << 16 | rawData_m[5] << 8 | rawData_m[4];
    srcNid_m = rawData_m[11] << 24 | rawData_m[10] << 16 | rawData_m[9] << 8 | rawData_m[8];
    outData_m.clear();
    outData_m.insert(outData_m.end(), rawData_m.begin() + 12, rawData_m.end());
    return true;
}

bool ZigTransLayer::integrity()
{
    uint8_t len = rawData_m[0];
    uint8_t sum = rawData_m[1];

    if (len != rawData_m.size())
    {
        return false;
    }

    Sum sumObj(vector<uint8_t>(rawData_m.begin() + 2, rawData_m.end()));
    if (sumObj != sum)
    {
        log_d("Sum check failed sum = %02x", sumObj.getSum());
        return false;
    }

    return true;
}


ZigAppLayer::ZigAppLayer(const vector<uint8_t> &src)
{
    type_m = src[0] << 8 | src[1];
    msgID_m = src[3] << 8 | src[2];
    outData_m.insert(outData_m.end(), src.begin() + 4, src.end());
}

ZigAppLayer::ZigAppLayer(uint16_t type, uint16_t msgID, const vector<uint8_t> &data)
{
    /* Type */
    outData_m.push_back(type >> 8);
    outData_m.push_back(type & 0xff);
    /* Msg ID */
    outData_m.push_back(msgID & 0xff);
    outData_m.push_back(msgID >> 8);
    /* Data */
    outData_m.insert(outData_m.end(), data.begin(), data.end());
}

vector<uint8_t> ZigAppLayer::getData()
{
    return outData_m;
}

uint16_t ZigAppLayer::getType()
{
    return type_m;
}

uint16_t ZigAppLayer::getMsgID()
{
    return msgID_m;
}



zigProtStack::zigProtStack(const vector<uint8_t> &msg):rawData_m(msg)
{

}

zigProtStack::~zigProtStack()
{

}

vector<uint8_t> zigProtStack::getData()
{
    return data_m;
}

uint16_t zigProtStack::getZid()
{
    log_d("zigProtStack::getZid", zid_m);
    return zid_m;
}

uint32_t zigProtStack::getSrcNid()
{
    return srcNid_m;
}

uint32_t zigProtStack::getTarNid()
{
    return tarNid_m;
}

uint16_t zigProtStack::getProtVer()
{
    return protVer_m;
}

uint16_t zigProtStack::getType()
{
    return type_m;
}

void zigProtStack::setProtVer(uint16_t ver)
{
    protVer_m = ver;
}

void zigProtStack::setSrcNid(uint32_t nid)
{
    srcNid_m = nid;
}

void zigProtStack::setTarNid(uint32_t nid)
{
    tarNid_m = nid;
}

void zigProtStack::setZid(uint16_t zid)
{
    zid_m = zid;
}

void zigProtStack::setType(uint16_t type)
{
    type_m = type;
}

void zigProtStack::setMsgID(uint16_t id)
{
    msgID_m = id;
}

bool zigProtStack::pack()
{
    ZigAppLayer appLayer(type_m, msgID_m, rawData_m);
    ZigTransLayer transLayer(appLayer.getData(), protVer_m, srcNid_m, tarNid_m);
    ZigNetLayer netLayer(transLayer.getData(), zid_m);
    data_m = netLayer.getData();
    return true;
}


bool zigProtStack::parse() 
{
    ZigNetLayer netLayer(rawData_m);
    if (!netLayer.unpack())
    {
        log_d("NetLayer unpack failed");
        return false;
    }

    ZigTransLayer transLayer(netLayer.getData());
    if (!transLayer.unpack())
    {
        log_d("TransLayer unpack failed");
        return false;
    }

    ZigAppLayer appLayer(transLayer.getData());

    data_m = appLayer.getData();
    type_m = appLayer.getType();
    zid_m = netLayer.getZid();
    srcNid_m = transLayer.getSrcNid();
    tarNid_m = transLayer.getTarNid();
    protVer_m = transLayer.getProtVer();
    msgID_m = appLayer.getMsgID();
    return true;
}




/* ----- End of file ----- */
