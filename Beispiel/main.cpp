#include <boost/dll.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>
#include <atomic>
#include "plugin_interface.h"


namespace dll = boost::dll;


static std::atomic<uint64_t> globalInstanceId{0};


struct LoadedPlugin {
    dll::shared_library handle;
    std::unique_ptr<IPlugin> plugin;
    std::string path, config;
    uint64_t instanceId = 0;
    
    // Default constructor
    LoadedPlugin() = default;
    
	// Move constructor - must be noexcept for std::vector operations
    LoadedPlugin(LoadedPlugin&& other) noexcept
        : handle(std::move(other.handle)),
          plugin(std::move(other.plugin)),
          path(std::move(other.path)),
          config(std::move(other.config)),
          instanceId(other.instanceId) {
        other.instanceId = 0;
    }
    
    // Move assignment
    LoadedPlugin& operator=(LoadedPlugin&& other) noexcept {
        if (this != &other) {
            // Unload current plugin if loaded
            if (isLoaded()) {
                unload();
            }
            
            handle = std::move(other.handle);
            plugin = std::move(other.plugin);
            path = std::move(other.path);
            config = std::move(other.config);
            instanceId = other.instanceId;
            other.instanceId = 0;
        }
        return *this;
    }
    
    // Copy operations deleted
    LoadedPlugin(const LoadedPlugin&) = delete;
    LoadedPlugin& operator=(const LoadedPlugin&) = delete;
    
    // Load plugin from file with configuration (includes initialization)
    bool load(const std::string& pluginPath, const std::string& pluginConfig = "") {
        // If already loaded, unload first
        if (isLoaded()) {
            unload();
        }
        
        path = pluginPath;
        config = pluginConfig;
        
        try {
            // Load the library
            handle = dll::shared_library(path);
            if (!handle.is_loaded()) {
                return false;
            }
            
            // Get the createPlugin function
            auto createFunc = handle.get<IPlugin*()>("createPlugin");
            if (!createFunc) {
                handle.unload();
                return false;
            }
            
            // Create plugin instance
            plugin.reset(createFunc());
            
            // Generate unique instance ID in the host
            instanceId = ++globalInstanceId;
            
            // Set the instance ID on the plugin
            plugin->setInstanceId(instanceId);
            
            // Initialize the plugin with its configuration
            if (!plugin->initialize(config)) {
                handle.unload();
                plugin.reset();
                return false;
            }
            
            return true;
        }
        catch (...) {
            handle.unload();
            return false;
        }
    }
    
    // Unload plugin
    void unload() {
        if (plugin) {
            plugin->reset();
            plugin.reset();
        }
        if (handle.is_loaded()) {
            handle.unload();
        }
        path.clear();
        config.clear();
        instanceId = 0;
    }
    
    // Check if plugin is loaded
    bool isLoaded() const {
        return plugin && handle.is_loaded();
    }
    
    // Update the plugin with data
    bool update(const std::vector<double>& data) {
        return plugin ? plugin->update(data) : false;
    }
    
    // Get results from the plugin
    std::vector<double> getResults() const {
        return plugin ? plugin->getResults() : std::vector<double>{};
    }
    
    // Get plugin state
    std::string getState() const {
        return plugin ? plugin->getState() : "No plugin";
    }
    
    // Reset the plugin
    void reset() {
        if (plugin) plugin->reset();
    }
    
    // Get plugin ID
    uint64_t getInstanceId() const {
        return instanceId;
    }
    
    // Get plugin name
    std::string getName() const {
        return plugin ? plugin->getName() : "Unknown";
    }
    
    // Destructor
    ~LoadedPlugin() {
        if (isLoaded()) {
            unload();
        }
    }
};

std::vector<double> generateTestData(size_t size, double minVal = 0.0, double maxVal = 100.0) {
    std::vector<double> data;
    data.reserve(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(minVal, maxVal);
    for (size_t i = 0; i < size; ++i) data.push_back(dis(gen));
    return data;
}

bool unloadPluginById(std::vector<LoadedPlugin>& plugins, uint64_t id) {
    auto it = std::find_if(plugins.begin(), plugins.end(), 
        [id](const LoadedPlugin& p) { return p.getInstanceId() == id; });
    if (it != plugins.end()) {
        it->reset();
        it->unload();  // Explicitly unload
        plugins.erase(it);
        return true;
    }
    return false;
}

bool unloadPluginByPath(std::vector<LoadedPlugin>& plugins, const std::string& path) {
    auto it = std::find_if(plugins.begin(), plugins.end(), 
        [&path](const LoadedPlugin& p) { return p.path == path; });
    if (it != plugins.end()) {
        it->reset();
        it->unload();  // Explicitly unload
        plugins.erase(it);
        return true;
    }
    return false;
}

bool reloadPlugin(std::vector<LoadedPlugin>& plugins, const std::string& path, const std::string& config = "") {
    unloadPluginByPath(plugins, path);
    try {
        LoadedPlugin plugin;
        if (plugin.load(path, config)) {
            plugins.push_back(std::move(plugin));
            return true;
        }
        return false;
    }
    catch (...) { return false; }
}

int main() {
    std::vector<LoadedPlugin> plugins;
    std::string pluginDir = "";
    
	plugins.emplace_back();
    if (!plugins.back().load(pluginDir + "plugin_maf.dll", "window_size=5")) {
        std::cerr << "Failed to load moving_average plugin" << std::endl;
        return 1;
    }
    
    plugins.emplace_back();
    if (!plugins.back().load(pluginDir + "plugin_counter.dll", "num_counters=3")) {
        std::cerr << "Failed to load counter plugin" << std::endl;
        return 1;
    }
    
    plugins.emplace_back();
    if (!plugins.back().load(pluginDir + "plugin_maf.dll", "window_size=10")) {
        std::cerr << "Failed to load second moving_average plugin" << std::endl;
        return 1;
    }
    
    plugins.emplace_back();
    if (!plugins.back().load(pluginDir + "plugin_counter.dll", "num_counters=5")) {
        std::cerr << "Failed to load second counter plugin" << std::endl;
        return 1;
    }
    
    // Show plugins
    std::cout << "Loaded plugins:" << std::endl;
    for (const auto& p : plugins) {
        std::cout << "  " << p.getName() << " (ID: " << p.getInstanceId() << ")" << std::endl;
    }
    
    // Simulate updates
    for (int iter = 0; iter < 5; ++iter) {
        std::cout << "\n--- Update " << iter + 1 << " ---" << std::endl;
        auto data = generateTestData(7, 1.0, 10.0);
        for (auto& p : plugins) {
            if (p.update(data)) {
                auto results = p.getResults();
                std::cout << p.getName() << " (ID: " << p.getInstanceId() << "): ";
                for (size_t i = 0; i < results.size(); ++i) {
                    std::cout << results[i];
                    if (i < results.size() - 1) std::cout << ", ";
                }
                std::cout << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Unload and reload demo
    std::cout << "\n--- Unloading plugin ID 1 ---" << std::endl;
    unloadPluginById(plugins, 1);
    std::cout << "Remaining plugins:" << std::endl;
    for (const auto& p : plugins) {
        std::cout << "  " << p.getName() << " (ID: " << p.getInstanceId() << ")" << std::endl;
    }
    
    std::cout << "\n--- Reloading moving_average ---" << std::endl;
    reloadPlugin(plugins, pluginDir + "/moving_average.dll", "window_size=7");
    
    std::cout << "\nFinal plugins:" << std::endl;
    for (const auto& p : plugins) {
        std::cout << "  " << p.getName() << " (ID: " << p.getInstanceId() << ")" << std::endl;
    }
    
    return 0;
}
