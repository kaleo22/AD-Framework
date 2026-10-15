#include "agent/dll_agent_adapter.h"
#include "game/game_state.h"

bool DllAgentAdapter::load(
    const std::string& dllPath,
    uint32_t mapWidth,
    uint32_t mapHeight)
{
    return player_.Load(
        dllPath.c_str(),
        mapWidth,
        mapHeight
    );
}

AgentAction DllAgentAdapter::getAction(
    const AgentObservation& observation)
{
    if (!player_.ready)
        return AgentAction::Wait;

    const WorldSnapshot& snapshot =
        observation.getApiSnapshot();

    const uint32_t apiAction =
        player_.Step(&snapshot);

    return fromAPIAction(apiAction);
}

void DllAgentAdapter::onActionExecuted(
    AgentAction action)
{
    switch (action)
    {
        case AgentAction::MoveUp:
            player_.lastAction = ACTION_MOVE_NORTH;
            break;

        case AgentAction::MoveDown:
            player_.lastAction = ACTION_MOVE_SOUTH;
            break;

        case AgentAction::MoveLeft:
            player_.lastAction = ACTION_MOVE_WEST;
            break;

        case AgentAction::MoveRight:
            player_.lastAction = ACTION_MOVE_EAST;
            break;

        case AgentAction::MoveUpLeft:
            player_.lastAction = ACTION_MOVE_NORTHWEST;
            break;

        case AgentAction::MoveUpRight:
            player_.lastAction = ACTION_MOVE_NORTHEAST;
            break;

        case AgentAction::MoveDownLeft:
            player_.lastAction = ACTION_MOVE_SOUTHWEST;
            break;

        case AgentAction::MoveDownRight:
            player_.lastAction = ACTION_MOVE_SOUTHEAST;
            break;

        case AgentAction::TakeFlag:
            player_.lastAction = ACTION_PICK_UP_FLAG;
            break;

        case AgentAction::DropFlag:
            player_.lastAction = ACTION_PUT_DOWN_FLAG;
            break;

        default:
            player_.lastAction = ACTION_WAIT;
            break;
    }
}