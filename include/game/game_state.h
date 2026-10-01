#pragma once

#include <vector>

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

