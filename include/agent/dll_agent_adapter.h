#pragma once
#include "agent/agent_interface.h"
#include "framework/agent_loader.h"
#include <cstdint>
#include <string>

class DllAgentAdapter : public IAgent
{
    public:
        bool load(
            const std::string& dllPath,
            uint32_t mapWidth,
            uint32_t mapHeight
        );
    
    AgentAction getAction(
        const AgentObservation& observation
    ) override;

    void onActionExecuted(AgentAction action) override;

    private:
        PlayerInstance player_;
};
