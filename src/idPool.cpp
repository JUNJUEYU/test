#include "idPool.h"
#include "log.h"

IdPool::IdPool()
{
    log_d("IdPool::IdPool");
}

IdPool::~IdPool()
{;}

uint16_t IdPool::randomZid()
{
    // uint16_t zid = devZidPool_m.back();
    // devZidPool_m.pop_back();
    return 0x2020;
}

uint32_t IdPool::randomNid()
{
    // uint32_t nid = devNidPool_m.back();
    // devNidPool_m.pop_back();
    return 0x30303030;
}



/* ----- End of file ----- */
