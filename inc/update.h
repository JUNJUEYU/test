#pragma once
#include <string>

using namespace std;

class Update{
    public:
        Update(const string &url);
        ~Update();
        bool run();

    private:
        string url_m;
};