#pragma once

#include "agent/agent_interface.h"

class ConsoleAgent : public IAgent
{
public:
    AgentAction getAction(
        const AgentObservation& observation
    ) override;
};