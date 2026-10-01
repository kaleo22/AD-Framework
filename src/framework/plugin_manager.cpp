#include "framework/plugin_manager.h"
#include <algorithm>
#include <utility>

bool PluginManager::loadPlugin(
    const std::string& path,
    const std::string& config)
{
    LoadedPlugin plugin;

    if (!plugin.load(path, config))
        return false;

    plugins.push_back(std::move(plugin));

    return true;
}

bool PluginManager::unloadPluginById(uint64_t id)
{
    auto it = std::find_if(
        plugins.begin(),
        plugins.end(),
        [id](const LoadedPlugin& plugin)
        {
            return plugin.getInstanceId() == id;
        }
    );

    if (it == plugins.end())
        return false;

    plugins.erase(it);

    return true;
}

bool PluginManager::unloadPluginByPath(
    const std::string& path)
{
    auto it = std::find_if(
        plugins.begin(),
        plugins.end(),
        [&path](const LoadedPlugin& plugin)
        {
            return plugin.getPath() == path;
        }
    );

    if (it == plugins.end())
        return false;

    plugins.erase(it);

    return true;
}

bool PluginManager::reloadPlugin(
    const std::string& path,
    const std::string& config)
{
    unloadPluginByPath(path);

    return loadPlugin(path, config);
}

void PluginManager::unloadAll()
{
    plugins.clear();
}

std::vector<LoadedPlugin>&
PluginManager::getPlugins()
{
    return plugins;
}

const std::vector<LoadedPlugin>&
PluginManager::getPlugins() const
{
    return plugins;
}