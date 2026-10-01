#include "game/game_engine.h"
#include "game/agent_types.h"


GameEngine::GameEngine()
{
    syncGameState();
}


void GameEngine::executeAction(
    AgentInstance& agent,
    AgentAction action)
{
    switch (action)
    {
        case AgentAction::MoveUp:
        case AgentAction::MoveDown:
        case AgentAction::MoveLeft:
        case AgentAction::MoveRight:
        case AgentAction::MoveUpLeft:
        case AgentAction::MoveUpRight:
        case AgentAction::MoveDownLeft:
        case AgentAction::MoveDownRight:
        {
            const Position delta =
                getMoveDelta(action);

            const Position current =
                agent.getPosition();

            const Position target{
                current.x + delta.x,
                current.y + delta.y
            };

            tryMove(agent, target);

            break;
        }

        case AgentAction::TakeFlag:
            tryTakeFlag(agent);
            break;

        case AgentAction::DropFlag:
            tryDropFlag(agent);
            break;

        case AgentAction::Wait:
            break;
    }
}


bool GameEngine::tryMove(
    AgentInstance& agent,
    Position target)
{
    if (target.x < 0 || target.y < 0)
        return false;

    if (
        target.x >= static_cast<int>(Map::width_) ||
        target.y >= static_cast<int>(Map::height_)
    )
        return false;

    const Cell targetCell =
        map_.getCell(
            static_cast<uint32_t>(target.x),
            static_cast<uint32_t>(target.y)
        );

    if (
        targetCell.getTerrain()
        == Cell::Terrain::Wall
    )
        return false;

    if (isOccupied(target))
        return false;

    agent.setPosition(target);

    syncGameState();

    return true;
}


bool GameEngine::tryTakeFlag(
    AgentInstance& agent)
{
    if (agent.isCarryingFlag())
        return false;

    const Position position =
        agent.getPosition();

    const Cell cell =
        map_.getCell(
            static_cast<uint32_t>(position.x),
            static_cast<uint32_t>(position.y)
        );

    if (
        cell.getContent()
        != Cell::Content::Flag
    )
        return false;

    map_.setCell(
        static_cast<uint32_t>(position.x),
        static_cast<uint32_t>(position.y),
        Cell(
            cell.getTerrain(),
            Cell::Content::None
        )
    );

    agent.setCarryingFlag(true);

    return true;
}


bool GameEngine::tryDropFlag(
    AgentInstance& agent)
{
    if (!agent.isCarryingFlag())
        return false;

    const Position position =
        agent.getPosition();

    const Cell cell =
        map_.getCell(
            static_cast<uint32_t>(position.x),
            static_cast<uint32_t>(position.y)
        );

    if (
        cell.getContent()
        != Cell::Content::None
    )
        return false;

    map_.setCell(
        static_cast<uint32_t>(position.x),
        static_cast<uint32_t>(position.y),
        Cell(
            cell.getTerrain(),
            Cell::Content::Flag
        )
    );

    agent.setCarryingFlag(false);

    return true;
}


bool GameEngine::isOccupied(
    Position position) const
{
    const Position position1 =
        agent1_.getPosition();

    const Position position2 =
        agent2_.getPosition();

    if (
        position1.x == position.x &&
        position1.y == position.y
    )
        return true;

    if (
        position2.x == position.x &&
        position2.y == position.y
    )
        return true;

    return false;
}


// void GameEngine::syncGameState()
// {
//     state.width =
//         static_cast<int>(Map::width_);
//
//     state.height =
//         static_cast<int>(Map::height_);
//
//     state.player1 =
//         agent1_.getPosition();
//
//     state.player2 =
//         agent2_.getPosition();
// }


AgentObservation GameEngine::buildObservation(
    const AgentInstance& agent) const
{
    AgentObservation observation;

    observation.setCarryingFlag(
        agent.isCarryingFlag()
    );

    observation.setRound(
        currentRound_
    );

    const Position agentPosition =
        agent.getPosition();

    constexpr int radius =
        AgentObservation::VISION_RADIUS;

    uint32_t index = 0;

    for (int dy = -radius; dy <= radius; ++dy)
    {
        for (int dx = -radius; dx <= radius; ++dx)
        {
            const int x =
                agentPosition.x + dx;

            const int y =
                agentPosition.y + dy;

            Cell visibleCell(
                Cell::Terrain::Wall,
                Cell::Content::None
            );

            if (
                x >= 0 &&
                y >= 0 &&
                x < static_cast<int>(Map::width_) &&
                y < static_cast<int>(Map::height_)
            )
            {
                visibleCell =
                    map_.getCell(
                        static_cast<uint32_t>(x),
                        static_cast<uint32_t>(y)
                    );

                const Position position1 =
                    agent1_.getPosition();

                const Position position2 =
                    agent2_.getPosition();

                if (
                    position1.x == x &&
                    position1.y == y
                )
                {
                    visibleCell = Cell(
                        visibleCell.getTerrain(),
                        agent1_.isCarryingFlag()
                            ? Cell::Content::AgentWithFlag
                            : Cell::Content::Agent
                    );
                }
                else if (
                    position2.x == x &&
                    position2.y == y
                )
                {
                    visibleCell = Cell(
                        visibleCell.getTerrain(),
                        agent2_.isCarryingFlag()
                            ? Cell::Content::AgentWithFlag
                            : Cell::Content::Agent
                    );
                }
            }

            observation.setVisibleCell(
                index,
                visibleCell.encode()
            );

            ++index;
        }
    }

    return observation;
}


void GameEngine::update()
{
    switch (state_)
    {
        case GameStates::Initializing:
        {
            if (initialize())
            {
                setState(
                    GameStates::WaitingForPlayers
                );
            }

            break;
        }

        case GameStates::WaitingForPlayers:
            if (
                agentController1_ != nullptr &&
                agentController2_ != nullptr
            )
            {
                setState(
                    GameStates::Ready
                );
            }

            break;

        case GameStates::Ready:
            setState(
                GameStates::Running
            );
            break;

        case GameStates::Running:
            updateRunning();
            checkGameFinished();
            break;

        case GameStates::Paused:
            break;

        case GameStates::Finished:
            break;

        case GameStates::Error:
            break;

        case GameStates::Shutdown:
            shutdown();
            break;
    }
}


const GameState& GameEngine::getState() const
{
    return state;
}


const Map& GameEngine::getMap() const
{
    return map_;
}

void GameEngine::syncGameState()
{
    state.width =
        static_cast<int>(Map::width_);

    state.height =
        static_cast<int>(Map::height_);

    state.player1 =
        agent1_.getPosition();

    state.player2 =
        agent2_.getPosition();
}

void GameEngine::initializeTestMap()
{
    map_.clear();

    for (uint32_t x = 0; x < Map::width_; ++x)
    {
        map_.setCell(
            x,
            0,
            Cell(
                Cell::Terrain::Wall,
                Cell::Content::None
            )
        );

        map_.setCell(
            x,
            Map::height_ - 1,
            Cell(
                Cell::Terrain::Wall,
                Cell::Content::None
            )
        );
    }

    for (uint32_t y = 0; y < Map::height_; ++y)
    {
        map_.setCell(
            0,
            y,
            Cell(
                Cell::Terrain::Wall,
                Cell::Content::None
            )
        );

        map_.setCell(
            Map::width_ - 1,
            y,
            Cell(
                Cell::Terrain::Wall,
                Cell::Content::None
            )
        );
    }

    map_.setCell(
        2,
        2,
        Cell(
            Cell::Terrain::Start,
            Cell::Content::None
        )
    );

    map_.setCell(
        13,
        7,
        Cell(
            Cell::Terrain::Start,
            Cell::Content::None
        )
    );

    map_.setCell(
        5,
        4,
        Cell(Cell::Terrain::Wall, Cell::Content::None)
    );

    map_.setCell(
        6,
        4,
        Cell(Cell::Terrain::Wall, Cell::Content::None)
    );

    map_.setCell(
        7,
        4,
        Cell(Cell::Terrain::Wall, Cell::Content::None)
    );

    map_.setCell(
        5,
        5,
        Cell(Cell::Terrain::Empty, Cell::Content::None)
    );

    map_.setCell(
        7,
        5,
        Cell(Cell::Terrain::Wall, Cell::Content::None)
    );

    map_.setCell(
        5,
        6,
        Cell(Cell::Terrain::Wall, Cell::Content::None)
    );

    map_.setCell(
        6,
        6,
        Cell(Cell::Terrain::Wall, Cell::Content::None)
    );

    map_.setCell(
        7,
        6,
        Cell(Cell::Terrain::Wall, Cell::Content::None)
    );

    map_.setCell(
        6,
        5,
        Cell(
            Cell::Terrain::Empty,
            Cell::Content::Flag
        )
    );
}

void GameEngine::setAgents(
    IAgent* agent1,
    IAgent* agent2)
{
    agentController1_ = agent1;
    agentController2_ = agent2;
}

bool GameEngine::initialize()
{
    initializeTestMap();

    syncGameState();

    return true;
}

void GameEngine::updateRunning()
{
    if (
        agentController1_ == nullptr ||
        agentController2_ == nullptr
    )
    {
        return;
    }

    AgentObservation observation1 =
        buildObservation(agent1_);

    AgentAction action1 =
        agentController1_->getAction(
            observation1
        );

    executeAction(
        agent1_,
        action1
    );


    AgentObservation observation2 =
        buildObservation(agent2_);

    AgentAction action2 =
        agentController2_->getAction(
            observation2
        );

    executeAction(
        agent2_,
        action2
    );


    ++currentRound_;

    syncGameState();
}

GameStates GameEngine::getGameState() const
{
    return state_;
}

void GameEngine::setState(GameStates newState)
{
    state_ = newState;
}

void GameEngine::checkGameFinished()
{
}

void GameEngine::shutdown()
{
}

AgentObservation GameEngine::getAgent1Observation() const
{
    return buildObservation(agent1_);
}