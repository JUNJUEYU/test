#include "protocolStack.h"
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
    outData_m.clear();
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
        Log::print("ZigNetLayer::unpack: invalid data", rawData_m);
        return false;
    }

    int rawDataLen = rawData_m.size();
    if (0x55 != rawData_m[rawDataLen - 1])
    {
        return false;
    }

    zid_m = (rawData_m[2] << 8) | rawData_m[3];

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
        Log::print("ZigTransLayer::integrity: invalid data", rawData_m);
        return false;
    }

    Sum sumObj(vector<uint8_t>(rawData_m.begin() + 2, rawData_m.end()));
    if (sumObj != sum)
    {
        Log::print("Sum check failed", sumObj.getSum());
        Log::print("ZigTransLayer::integrity: invalid data", rawData_m);
        return false;
    }

    return true;
}

ZigAppLayer::ZigAppLayer(const vector<uint8_t> &src)
{
    type_m = src[0] << 8 | src[1];
    outData_m.insert(outData_m.end(), src.begin() + 4, src.end());
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

ZigAppLayer::ZigAppLayer(uint8_t result, uint32_t nid)
{
    /* Net Access */
    outData_m.push_back(1);
    /* Static */
    outData_m.push_back(2);
    /* Uid */
    uint16_t uid = UID::getInstance().getUid();
    outData_m.push_back(uid & 0xff);
    outData_m.push_back((uid >> 8) & 0xff);
    /* Result */
    outData_m.push_back(result);
    /* Nid */
    outData_m.push_back(nid & 0xff);
    outData_m.push_back((nid >> 8) & 0xff);
    outData_m.push_back((nid >> 16) & 0xff);
    outData_m.push_back((nid >> 24) & 0xff);
}

ZigAppLayer::ZigAppLayer(uint8_t result, uint32_t nid, uint16_t zid)
{
    /* Net Access */
    outData_m.push_back(1);
    /* Dynamic */
    outData_m.push_back(1);
    /* Uid */
    uint16_t uid = UID::getInstance().getUid();
    outData_m.push_back(uid & 0xff);
    outData_m.push_back((uid >> 8) & 0xff);
    /* Result */
    outData_m.push_back(result);
    /* Nid */
    outData_m.push_back(nid & 0xff);
    outData_m.push_back((nid >> 8) & 0xff);
    outData_m.push_back((nid >> 16) & 0xff);
    outData_m.push_back((nid >> 24) & 0xff);
    /* Zid */
    outData_m.push_back(zid & 0xff);
    outData_m.push_back((zid >> 8) & 0xff);
}

// vector<uint8_t> ProtocolStack::packNetAccessAck(uint8_t result, uint32_t nid)
// {
//     SubDeviceManager &subDevManager = SubDeviceManager::getInstance();
//     ZigAppLayer appLayer(result, nid);
//     ZigTransLayer transLayer(appLayer.getData(), 0, 0, 0);
// }

// vector<uint8_t> ProtocolStack::packNetAccessAck(uint8_t result, uint32_t nid, uint16_t zid)
// {
//     SubDeviceManager &subDevManager = SubDeviceManager::getInstance();
//     ZigAppLayer appLayer(result, nid, zid);
//     ZigTransLayer transLayer(appLayer.getData(),
//                              subDevManager.getVer(nid),
//                              nid,
//                              GW_NID);
//     ZigNetLayer netLayer(transLayer.getData(), zid);
//     return netLayer.getData();
// }

// MsgDev *ProtocolStack::devAccessNet(ZigAppLayer &appLayer, uint16_t ver, const vector<uint8_t> &src)
// {
//     MsgDev *msgDev = NULL;
//     SubDeviceManager &subDevManager = SubDeviceManager::getInstance();

//     if ("Net Access Dynamic" == appLayer.getType())
//     {
//         Log::print("Net Access Dynamic");
//         if (!subDevManager.creatSubDev(appLayer.getSn(), ver))
//         {
//             msgDev = new MsgDev(packNetAccessAck(1, 0, 0));
//             goto exit;
//         }
//         msgDev = new MsgDev(packNetAccessAck(0,
//                                              subDevManager.getNid(appLayer.getSn()),
//                                              subDevManager.getZid(appLayer.getSn())));
//     }
//     else if ("Net Access Static" == appLayer.getType())
//     {
//         Log::print("Net Access Static");
//         uint16_t zid = src[20] << 8 | src[21];
//         if (!subDevManager.creatSubDev(appLayer.getSn(), ver))
//         {
//             msgDev = new MsgDev(packNetAccessAck(1, 0));
//             goto exit;
//         }
//         msgDev = new MsgDev(packNetAccessAck(0, subDevManager.getNid(appLayer.getSn())));
//     }
//     else
//     {
//     }

// exit:
//     return msgDev;
// }

// Msg *ProtocolStack::getDeviceMsg(const vector<uint8_t> &src)
// {
//     Msg *msg = NULL;
//     string ret;

//     ZigNetLayer netLayer(src);
//     if (!netLayer.unpack())
//     {
//         Log::print("NetLayer unpack failed");
//         return NULL;
//     }

//     ZigTransLayer transLayer(netLayer.getData());
//     if (!transLayer.unpack())
//     {
//         Log::print("TransLayer unpack failed");
//         return NULL;
//     }

//     SubDeviceManager &subDevManager = SubDeviceManager::getInstance();
//     if (!subDevManager.verify(transLayer.getSrcNid()))
//     {
//         Log::print("Verify failed");
//         return NULL;
//     }

//     ZigAppLayer appLayer(transLayer.getData());
//     msg = devAccessNet(appLayer, transLayer.getProtVer(), appLayer.getData());

//     return msg;
// }

// Msg *ProtocolStack::getDeviceMsg(const string &src)
// {
// }

ProtocolStackZigbee::ProtocolStackZigbee(const vector<uint8_t> &msg)
{
    ZigNetLayer netLayer(msg);
    if (!netLayer.unpack())
    {
        Log::print("NetLayer unpack failed");
        return;
    }

    ZigTransLayer transLayer(netLayer.getData());
    if (!transLayer.unpack())
    {
        Log::print("TransLayer unpack failed");
        return ;
    }

    ZigAppLayer appLayer(transLayer.getData());

    data_m = appLayer.getData();
    type_m = appLayer.getType();
    zid_m = netLayer.getZid();
    srcNid_m = transLayer.getSrcNid();
    tarNid_m = transLayer.getTarNid();
    protVer_m = transLayer.getProtVer();
}

ProtocolStackZigbee::~ProtocolStackZigbee()
{

}

vector<uint8_t> ProtocolStackZigbee::getData()
{

}

uint16_t ProtocolStackZigbee::getZid()
{

}

uint32_t ProtocolStackZigbee::getSrcNid()
{

}

uint32_t ProtocolStackZigbee::getTarNid()
{

}

uint16_t ProtocolStackZigbee::getProtVer()
{

}

uint16_t ProtocolStackZigbee::getType()
{

}


/* ----- End of file ----- */
