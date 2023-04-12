#pragma once
#include <vector>
#include <string>
#include <stdint.h>
using namespace std;

class IdPool{
    public:
        IdPool();
        ~IdPool();

        uint16_t randomZid();
        uint32_t randomNid();
        
    private:
        vector<uint16_t> devZidPool_m;
        vector<uint32_t> devNidPool_m;
};

/* ----- End of file ----- */
