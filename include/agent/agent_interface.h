#pragma once

#include "game/game_state.h"
#include "game/agent_types.h"

class IAgent
{
public:
    virtual ~IAgent() = default;

    virtual AgentAction getAction(
        const AgentObservation& observation
    ) = 0;

    virtual void onActionExecuted(AgentAction action)
    {
        (void)action;
    }
};