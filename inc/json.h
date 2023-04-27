#pragma once
#include "cJSON.h"
#include "string"
using namespace std;
class Json{
    public:
        Json(const string& text):root_m(NULL){
            root_m = cJSON_Parse(text.c_str());
        };
        ~Json(){
            if(root_m){
                cJSON_Delete(root_m);
            }
        };
        cJSON *root(){
            return root_m;
        };
    private:
        cJSON *root_m;
};