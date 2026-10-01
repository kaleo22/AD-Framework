#include "plugin_interface.h"
#include <vector>
#include <numeric>
#include <sstream>

class MovingAveragePlugin : public IPlugin {
private:
    std::vector<double> buffer;
    size_t bufferSize = 10;
    size_t currentIndex = 0;
    bool initialized = false;
    uint64_t instanceId = 0;

public:
    MovingAveragePlugin() = default;
    
    std::string getName() const override { return "Moving Average Plugin"; }
    
    bool initialize(const std::string& config) override {
        try {
            if (config.empty()) bufferSize = 10;
            else {
                size_t pos = config.find("window_size=");
                if (pos != std::string::npos) bufferSize = std::stoul(config.substr(pos + 12));
            }
            buffer.resize(bufferSize, 0.0);
            currentIndex = 0;
            initialized = true;
            return true;
        }
        catch (...) { return false; }
    }
    
    bool update(const std::vector<double>& inputData) override {
        if (!initialized || inputData.empty()) return true;
        try {
            for (double value : inputData) {
                buffer[currentIndex] = value;
                currentIndex = (currentIndex + 1) % bufferSize;
            }
            return true;
        }
        catch (...) { return false; }
    }
    
    std::vector<double> getResults() const override {
        if (!initialized) return {};
        std::vector<double> results;
        results.reserve(buffer.size() + 1);
        results.push_back(std::accumulate(buffer.begin(), buffer.end(), 0.0) / buffer.size());
        results.insert(results.end(), buffer.begin(), buffer.end());
        return results;
    }
    
    std::string getState() const override {
        std::ostringstream oss;
        oss << "Instance ID: " << instanceId 
            << ", Window Size: " << bufferSize 
            << ", Initialized: " << (initialized ? "Yes" : "No");
        return oss.str();
    }
    
    void reset() override {
        std::fill(buffer.begin(), buffer.end(), 0.0);
        currentIndex = 0;
    }
    
    void setInstanceId(uint64_t id) override { instanceId = id; }
    uint64_t getInstanceId() const override { return instanceId; }
};

extern "C" {
    IPlugin* createPlugin() { return new MovingAveragePlugin(); }
    void destroyPlugin(IPlugin* plugin) { delete plugin; }
}
