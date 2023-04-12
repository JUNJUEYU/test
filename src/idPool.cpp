#include "idPool.h"
#include "log.h"

IdPool::IdPool()
{
    Log::print("IdPool::IdPool");
}

IdPool::~IdPool()
{;}

uint16_t IdPool::randomZid()
{
    uint16_t zid = devZidPool_m.back();
    devZidPool_m.pop_back();
    return zid;
}

uint32_t IdPool::randomNid()
{
    uint32_t nid = devNidPool_m.back();
    devNidPool_m.pop_back();
    return nid;
}



/* ----- End of file ----- */
