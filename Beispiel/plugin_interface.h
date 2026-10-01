#pragma once


#include <string>
#include <vector>
#include <cstdint>


class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual std::string getName() const = 0;
    virtual bool initialize(const std::string& config) = 0;
    virtual bool update(const std::vector<double>& inputData) = 0;
    virtual std::vector<double> getResults() const = 0;
    virtual std::string getState() const = 0;
    virtual void reset() = 0;
    virtual void setInstanceId(uint64_t id) = 0;
    virtual uint64_t getInstanceId() const = 0;
};


typedef IPlugin* (*CreatePluginFunc)();
typedef void (*DestroyPluginFunc)(IPlugin*);
