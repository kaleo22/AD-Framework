#pragma once

#include <vector>
#include <cstdint>
#include "agent/agent_api.h"

struct Position
{
    int x = 0;
    int y = 0;
};

enum class AgentAction
{
    Wait,

    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,

    MoveUpLeft,
    MoveUpRight,
    MoveDownLeft,
    MoveDownRight,

    TakeFlag,
    DropFlag
};

inline Position getMoveDelta(AgentAction action)
{
    switch (action)
    {
        case AgentAction::MoveUp:
            return {0, -1};

        case AgentAction::MoveDown:
            return {0, 1};

        case AgentAction::MoveLeft:
            return {-1, 0};

        case AgentAction::MoveRight:
            return {1, 0};

        case AgentAction::MoveUpLeft:
            return {-1, -1};

        case AgentAction::MoveUpRight:
            return {1, -1};

        case AgentAction::MoveDownLeft:
            return {-1, 1};

        case AgentAction::MoveDownRight:
            return {1, 1};

        default:
            return {0, 0};
    }
}

inline AgentAction fromAPIAction(uint32_t apiAction) {
    switch (apiAction) {
        case ACTION_WAIT:
            return AgentAction::Wait;

        case ACTION_MOVE_NORTH:
            return AgentAction::MoveUp;

        case ACTION_MOVE_SOUTH:
            return AgentAction::MoveDown;

        case ACTION_MOVE_WEST:
            return AgentAction::MoveLeft;

        case ACTION_MOVE_EAST:
            return AgentAction::MoveRight;

        case ACTION_MOVE_NORTHWEST:
            return AgentAction::MoveUpLeft;

        case ACTION_MOVE_NORTHEAST:
            return AgentAction::MoveUpRight;

        case ACTION_MOVE_SOUTHWEST:
            return AgentAction::MoveDownLeft;

        case ACTION_MOVE_SOUTHEAST:
            return AgentAction::MoveDownRight;

        case ACTION_PICK_UP_FLAG:
            return AgentAction::TakeFlag;

        case ACTION_PUT_DOWN_FLAG:
            return AgentAction::DropFlag;

        default:
            return AgentAction::Wait;
    }
}


struct GameState
{
    int width = 16;
    int height = 10;

    Position player1{2, 2};
    Position player2{13, 7};
};

enum class GameStates
{
    Initializing,
    WaitingForPlayers,
    Ready,
    Running,
    Paused,
    Finished,
    Error,
    Shutdown
};

