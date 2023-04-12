#include "plugin.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <dlfcn.h>
#include "log.h"


static void findAllPlugins(const std::string& path, std::list<std::string>& pluginNames)
{
    DIR *dir;
    struct dirent *ptr;
    if ((dir = opendir(path.c_str())) == NULL)
    {
        Log::print("Open dir error...");
        return;
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_type == 8) //file
        {
            std::string fileName = ptr->d_name;
            if (fileName.find(".so") != std::string::npos)
            {
                pluginNames.push_back(fileName);
            }
        }
    }
    closedir(dir);
}

PluginManager::PluginManager()
{
    findAllPlugins("./plugins", pluginNames);
}

void PluginManager::loadPlugins()
{
    for (std::list<std::string>::iterator it = pluginNames.begin(); it != pluginNames.end(); it++)
    {
        std::string pluginName = *it;
        std::string pluginPath = "./plugins/" + pluginName;
        void *handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
        if (handle == NULL)
        {
            std::cout << "dlopen " << pluginPath << " failed" << std::endl;
            continue;
        }
        ProtAPI *(*create)();
        create = (ProtAPI * (*)()) dlsym(handle, "create");
        if (create == NULL)
        {
            std::cout << "dlsym create failed" << std::endl;
            continue;
        }
        ProtAPI *plugin = create();
        plugins.push_back(plugin);
    }
}



void PluginManager::unloadPlugins()
{
    for (std::list<ProtAPI *>::iterator it = plugins.begin(); it != plugins.end(); it++)
    {
        ProtAPI *plugin = *it;
        delete plugin;
    }
}


ProtAPI *PluginManager::getProt(uint16_t version)
{
    for (std::list<ProtAPI *>::iterator it = plugins.begin(); it != plugins.end(); it++)
    {
        ProtAPI *plugin = *it;
        if (plugin->getVersion() == version)
        {
            return plugin;
        }
    }
    return NULL;
}
/* ----- End of file ----- */
