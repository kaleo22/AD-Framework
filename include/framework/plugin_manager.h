#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "framework/loaded_plugin.h"

class PluginManager
{
public:
    bool loadPlugin(
        const std::string& path,
        const std::string& config = ""
    );

    bool unloadPluginById(uint64_t id);

    bool unloadPluginByPath(
        const std::string& path
    );

    bool reloadPlugin(
        const std::string& path,
        const std::string& config = ""
    );

    void unloadAll();

    std::vector<LoadedPlugin>& getPlugins();

    const std::vector<LoadedPlugin>& getPlugins() const;

private:
    std::vector<LoadedPlugin> plugins;
};