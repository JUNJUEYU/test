#pragma once

#include <iostream>
#include <string>
#include <list>


//插件
class Plugin {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

//插件管理器
class PluginManager {
public:
    PluginManager();
    void loadPlugins();
    void startPlugins();
    void stopPlugins() ;
    void unloadPlugins() ;
private:
        std::list<std::string> pluginNames;
        std::list<Plugin*> plugins;
};

/* ----- End of file ----- */
