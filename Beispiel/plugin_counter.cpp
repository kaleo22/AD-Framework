#include "plugin_interface.h"
#include <vector>
#include <sstream>

class CounterPlugin : public IPlugin {
private:
    std::vector<int> counters;
    size_t numCounters = 1;
    bool initialized = false;
    uint64_t instanceId = 0;

public:
    CounterPlugin() = default;
    
    std::string getName() const override { return "Counter Plugin"; }
    
    bool initialize(const std::string& config) override {
        try {
            if (config.empty()) numCounters = 1;
            else {
                size_t pos = config.find("num_counters=");
                if (pos != std::string::npos) numCounters = std::stoul(config.substr(pos + 13));
            }
            counters.resize(numCounters, 0);
            initialized = true;
            return true;
        }
        catch (...) { return false; }
    }
    
    bool update(const std::vector<double>& inputData) override {
        if (!initialized || inputData.empty()) return true;
        try {
            for (size_t i = 0; i < inputData.size(); ++i) {
                counters[i % numCounters] += static_cast<int>(inputData[i]);
            }
            return true;
        }
        catch (...) { return false; }
    }
    
    std::vector<double> getResults() const override {
        if (!initialized) return {};
        std::vector<double> results;
        results.reserve(counters.size() + 1);
        long long total = 0;
        for (int c : counters) { results.push_back(c); total += c; }
        results.push_back(total);
        return results;
    }
    
    std::string getState() const override {
        std::ostringstream oss;
        oss << "Instance ID: " << instanceId 
            << ", Counters: " << numCounters 
            << ", Initialized: " << (initialized ? "Yes" : "No");
        return oss.str();
    }
    
    void reset() override { std::fill(counters.begin(), counters.end(), 0); }
    
    void setInstanceId(uint64_t id) override { instanceId = id; }
    uint64_t getInstanceId() const override { return instanceId; }
};

extern "C" {
    IPlugin* createPlugin() { return new CounterPlugin(); }
    void destroyPlugin(IPlugin* plugin) { delete plugin; }
}