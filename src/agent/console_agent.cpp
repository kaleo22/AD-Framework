#include "agent/console_agent.h"

#include <iostream>
#include <string>

AgentAction ConsoleAgent::getAction(
    const AgentObservation& observation)
{
    std::cout
        << "\nRound: "
        << observation.getRound()
        << '\n';

    std::cout
        << "Carrying flag: "
        << (
            observation.isCarryingFlag()
                ? "yes"
                : "no"
        )
        << '\n';

    std::cout
        << "Action: ";

    std::string input;
    std::cin >> input;

    if (input == "w")
        return AgentAction::MoveUp;

    if (input == "s")
        return AgentAction::MoveDown;

    if (input == "a")
        return AgentAction::MoveLeft;

    if (input == "d")
        return AgentAction::MoveRight;

    if (input == "q")
        return AgentAction::MoveUpLeft;

    if (input == "e")
        return AgentAction::MoveUpRight;

    if (input == "y")
        return AgentAction::MoveDownLeft;

    if (input == "c")
        return AgentAction::MoveDownRight;

    if (input == "take")
        return AgentAction::TakeFlag;

    if (input == "drop")
        return AgentAction::DropFlag;

    return AgentAction::Wait;
}