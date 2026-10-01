#pragma once

#include "agent/agent_interface.h"

class WaitAgent : public IAgent
{
public:
    AgentAction getAction(
        const AgentObservation&)
        override
    {
        return AgentAction::Wait;
    }
};