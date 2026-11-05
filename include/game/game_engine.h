#pragma once

#include <cstdint>

#include "game/game_state.h"
#include "game/agent_types.h"
#include "game/build_map.h"
#include "framework/plugin_manager.h"
#include "agent/agent_interface.h"


class GameEngine
{
public:

    void setAgents(
        IAgent* agent1,
        IAgent* agent2
    );

    GameEngine();

    void update();

    GameStates getGameState() const;

    const GameState& getState() const;

    const Map& getMap() const;

    AgentObservation getAgent1Observation() const;

private:

    IAgent* agentController1_ = nullptr;
    IAgent* agentController2_ = nullptr;
    bool initialize();

    void initializeTestMap();

    void setState(
        GameStates newState
    );

    void updateRunning();

    void checkGameFinished();

    void shutdown();

    AgentAction executeAction(
        GameAgent& agent,
        AgentAction action
    );

    bool tryMove(
        GameAgent& agent,
        Position target
    );

    bool tryTakeFlag(
        GameAgent& agent
    );

    bool tryDropFlag(
        GameAgent& agent
    );

    bool isOccupied(
        Position position
    ) const;

    AgentObservation buildObservation(
        const GameAgent& agent
    ) const;

    void syncGameState();

private:
    static constexpr Position BASE_1{2, 2};
    static constexpr Position BASE_2{13, 7};

    Map map_;

    PluginManager pluginManager_;

    GameAgent agent1_{
        1,
        BASE_1
    };

    GameAgent agent2_{
        2,
        BASE_2
    };

    GameState state;

    GameStates state_ =
        GameStates::Initializing;

    uint32_t currentRound_ = 0;
};
