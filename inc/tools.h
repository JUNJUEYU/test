#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <pthread.h>

using namespace std;

class UtcTime{
    public:
        UtcTime();
        ~UtcTime();
        static uint32_t getUtcTimeStamp();
};


class Bcd{
    public:
        Bcd(const vector<uint8_t> &data):bcdData_m(data){};
        Bcd(const string &data):decData_m(data){};
        ~Bcd(){};

        string toDecString();
        vector<uint8_t> toBcd();

    private:
        vector<uint8_t> bcdData_m;
        string decData_m;
};

class Sum{
    public:
        Sum(const vector<uint8_t> &data):data_m(data){
            sum_m = check();
        };
        ~Sum(){};
        bool operator==(uint8_t sum);
        bool operator!=(uint8_t sum);
        uint8_t getSum(){return sum_m;};

    private:
        uint8_t check();
        vector<uint8_t> data_m;
        uint8_t sum_m;
};

class UID{
    public:
        static UID& getInstance(){
            static UID instance;
            return instance;
        }
        
        uint16_t getUid(){
            uint16_t ret;
            pthread_mutex_lock(&mutex_m);
            ret = uid_m++;
            pthread_mutex_unlock(&mutex_m);
            return ret;
        };

    private:
        UID(){
             pthread_mutex_init(&mutex_m, NULL);
             uid_m = 0;
        }
        ~UID(){};
        UID(const UID&){};
        UID& operator=(const UID&){};
    private:
        pthread_mutex_t mutex_m;
        uint16_t uid_m;
};


class Lock{
    public:
        Lock(pthread_mutex_t &mutex):mutex_m(mutex){
            pthread_mutex_lock(&mutex_m);
        };
        ~Lock(){
            pthread_mutex_unlock(&mutex_m);
        };
    private:
        pthread_mutex_t &mutex_m;
};

/* ----- End of file ----- */
