#pragma once

#include <boost/dll.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "plugins/plugin_interface.h"

class LoadedPlugin
{
public:
    LoadedPlugin() = default;

    LoadedPlugin(LoadedPlugin&& other) noexcept;
    LoadedPlugin& operator=(LoadedPlugin&& other) noexcept;

    LoadedPlugin(const LoadedPlugin&) = delete;
    LoadedPlugin& operator=(const LoadedPlugin&) = delete;

    ~LoadedPlugin();

    bool load(
        const std::string& pluginPath,
        const std::string& pluginConfig = ""
    );

    void unload();

    bool isLoaded() const;

    bool update(const std::vector<double>& data);

    std::vector<double> getResults() const;

    std::string getState() const;

    void reset();

    uint64_t getInstanceId() const;

    std::string getName() const;

    const std::string& getPath() const;

private:
    boost::dll::shared_library handle;
    std::unique_ptr<IPlugin> plugin;

    std::string path;
    std::string config;

    uint64_t instanceId = 0;
};