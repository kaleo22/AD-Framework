#include "framework/loaded_plugin.h"

#include <atomic>
#include <utility>

static std::atomic<uint64_t> globalInstanceId{0};

LoadedPlugin::LoadedPlugin(LoadedPlugin&& other) noexcept
    : handle(std::move(other.handle)),
      plugin(std::move(other.plugin)),
      path(std::move(other.path)),
      config(std::move(other.config)),
      instanceId(other.instanceId)
{
    other.instanceId = 0;
}

LoadedPlugin& LoadedPlugin::operator=(LoadedPlugin&& other) noexcept
{
    if (this != &other)
    {
        unload();

        handle = std::move(other.handle);
        plugin = std::move(other.plugin);
        path = std::move(other.path);
        config = std::move(other.config);
        instanceId = other.instanceId;

        other.instanceId = 0;
    }

    return *this;
}

LoadedPlugin::~LoadedPlugin()
{
    unload();
}

bool LoadedPlugin::load(
    const std::string& pluginPath,
    const std::string& pluginConfig)
{
    unload();

    path = pluginPath;
    config = pluginConfig;

    try
    {
        handle = boost::dll::shared_library(path);

        if (!handle.is_loaded())
            return false;

        auto createFunc =
            handle.get<IPlugin*()>("createPlugin");

        plugin.reset(createFunc());

        if (!plugin)
        {
            handle.unload();
            return false;
        }

        instanceId = ++globalInstanceId;

        plugin->setInstanceId(instanceId);

        if (!plugin->initialize(config))
        {
            plugin.reset();
            handle.unload();
            return false;
        }

        return true;
    }
    catch (...)
    {
        plugin.reset();

        if (handle.is_loaded())
            handle.unload();

        return false;
    }
}

void LoadedPlugin::unload()
{
    if (plugin)
    {
        plugin->reset();
        plugin.reset();
    }

    if (handle.is_loaded())
    {
        handle.unload();
    }

    path.clear();
    config.clear();
    instanceId = 0;
}

bool LoadedPlugin::isLoaded() const
{
    return plugin && handle.is_loaded();
}

bool LoadedPlugin::update(const std::vector<double>& data)
{
    return plugin
        ? plugin->update(data)
        : false;
}

std::vector<double> LoadedPlugin::getResults() const
{
    return plugin
        ? plugin->getResults()
        : std::vector<double>{};
}

std::string LoadedPlugin::getState() const
{
    return plugin
        ? plugin->getState()
        : "No plugin";
}

void LoadedPlugin::reset()
{
    if (plugin)
        plugin->reset();
}

uint64_t LoadedPlugin::getInstanceId() const
{
    return instanceId;
}

std::string LoadedPlugin::getName() const
{
    return plugin
        ? plugin->getName()
        : "Unknown";
}

const std::string& LoadedPlugin::getPath() const
{
    return path;
}