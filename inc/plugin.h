#pragma once

#include <iostream>
#include <string>
#include <list>
#include "pluginAPI.h"
using namespace std;

//插件
class Plugin {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

//插件注册
class PluginManager {
public:
    PluginManager();
    void loadPlugins();
    void unloadPlugins();
    ProtAPI *getProt(uint16_t version);

private:
        std::list<std::string> pluginNames;
        std::list<ProtAPI *> plugins;
};

/* ----- End of file ----- */
