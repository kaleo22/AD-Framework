#include "game/game_engine.h"

namespace
{
uint32_t flagToApi(FlagId flag, FlagId ownFlag)
{
    if (flag == FlagId::None)
        return CELLCONTENT_NONE;

    return flag == ownFlag ? CELLCONTENT_OWN_FLAG : CELLCONTENT_ENEMY_FLAG;
}
}


GameEngine::GameEngine()
{
    syncGameState();
}


AgentAction GameEngine::executeAction(
    GameAgent& agent,
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

            return tryMove(agent, target)
                ? action
                : AgentAction::Wait;
        }

        case AgentAction::TakeFlag:
            return tryTakeFlag(agent)
                ? action
                : AgentAction::Wait;

        case AgentAction::DropFlag:
            return tryDropFlag(agent)
                ? action
                : AgentAction::Wait;

        case AgentAction::Wait:
        default:
            return AgentAction::Wait;
    }

    return AgentAction::Wait;
}


bool GameEngine::tryMove(GameAgent& agent, Position target)
{
    const CellState surface = describeCell(map_.getCell(target.x, target.y)).surface;
    if (surface == CellState::Wall || surface == CellState::Unknown || isOccupied(target))
        return false;

    agent.setPosition(target);
    syncGameState();
    return true;
}


bool GameEngine::tryTakeFlag(GameAgent& agent)
{
    if (agent.isCarryingFlag())
        return false;

    const Position position = agent.getPosition();
    const CellInfo& cell = describeCell(map_.getCell(position.x, position.y));
    if (cell.groundFlag == FlagId::None)
        return false;

    map_.setCell(position.x, position.y, cell.surface);
    agent.setCarriedFlag(cell.groundFlag);
    return true;
}


bool GameEngine::tryDropFlag(GameAgent& agent)
{
    if (!agent.isCarryingFlag())
        return false;

    const Position position = agent.getPosition();
    const CellInfo& cell = describeCell(map_.getCell(position.x, position.y));
    if (cell.groundFlag != FlagId::None)
        return false;

    const CellState dropped = makeCellState(cell.surface, agent.getCarriedFlag());
    if (dropped == CellState::Unknown)
        return false;

    map_.setCell(position.x, position.y, dropped);
    agent.setCarriedFlag(FlagId::None);
    return true;
}


bool GameEngine::isOccupied(Position position) const
{
    return position == agent1_.getPosition() || position == agent2_.getPosition();
}


AgentObservation GameEngine::buildObservation(const GameAgent& agent) const
{
    const bool isPlayer1 = agent.getId() == agent1_.getId();
    const FlagId ownFlag = isPlayer1 ? FlagId::Team1 : FlagId::Team2;
    const Position ownBase = isPlayer1 ? BASE_1 : BASE_2;
    const Position enemyBase = isPlayer1 ? BASE_2 : BASE_1;
    const GameAgent& enemy = isPlayer1 ? agent2_ : agent1_;
    const Position agentPosition = agent.getPosition();
    const Position enemyPosition = enemy.getPosition();

    AgentObservation observation(ownFlag);
    observation.setRound(currentRound_);

    constexpr int radius = AgentObservation::VISION_RADIUS;
    uint32_t index = 0;
    for (int dy = -radius; dy <= radius; ++dy)
    {
        for (int dx = -radius; dx <= radius; ++dx, ++index)
        {
            const Position position{agentPosition.x + dx, agentPosition.y + dy};
            CellState visibleCell = map_.getCell(position.x, position.y);
            const bool isSelf = position == agentPosition;

            if (isSelf)
                visibleCell = withAgent(visibleCell, agent.getCarriedFlag());
            else if (position == enemyPosition)
                visibleCell = withAgent(visibleCell, enemy.getCarriedFlag());

            observation.setVisibleCell(index, visibleCell);

            const CellInfo& cell = describeCell(visibleCell);
            uint32_t apiCell = CELLTYPE_UNKNOWN;
            switch (cell.surface)
            {
                case CellState::Ground:
                    apiCell = CELLTYPE_FLOOR;
                    break;
                case CellState::Wall:
                    apiCell = CELLTYPE_WALL;
                    break;
                case CellState::Base:
                    apiCell = CELLTYPE_TARGET;
                    if (position == ownBase)
                        apiCell |= CELLCONTENT_OWN_TARGET;
                    else if (position == enemyBase)
                        apiCell |= CELLCONTENT_ENEMY_TARGET;
                    break;
                default:
                    break;
            }

            if (cell.occupied)
                apiCell |= isSelf ? CELLCONTENT_SELF : CELLCONTENT_ENEMY;

            apiCell |= flagToApi(cell.groundFlag, ownFlag);
            apiCell |= flagToApi(cell.carriedFlag, ownFlag);
            observation.setApiCell(index, apiCell);
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

    state.player1CarriedFlag = agent1_.getCarriedFlag();
    state.player2CarriedFlag = agent2_.getCarriedFlag();
}

void GameEngine::initializeTestMap()
{
    map_.clear();
    constexpr int width = static_cast<int>(Map::width_);
    constexpr int height = static_cast<int>(Map::height_);

    for (int x = 0; x < width; ++x)
    {
        map_.setCell(x, 0, CellState::Wall);
        map_.setCell(x, height - 1, CellState::Wall);
    }

    for (int y = 0; y < height; ++y)
    {
        map_.setCell(0, y, CellState::Wall);
        map_.setCell(width - 1, y, CellState::Wall);
    }

    constexpr Position innerWalls[]{
        {5, 4}, {6, 4}, {7, 4}, {7, 5}, {5, 6}, {6, 6}, {7, 6}
    };
    for (const Position wall : innerWalls)
        map_.setCell(wall.x, wall.y, CellState::Wall);

    map_.setCell(BASE_1.x, BASE_1.y, CellState::BaseFlag1);
    map_.setCell(BASE_2.x, BASE_2.y, CellState::BaseFlag2);
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

    const AgentAction executed1 =
        executeAction(
            agent1_,
            action1
        );

    agentController1_->onActionExecuted(executed1);


    AgentObservation observation2 =
        buildObservation(agent2_);

    AgentAction action2 =
        agentController2_->getAction(
            observation2
        );

    const AgentAction executed2 =
        executeAction(
            agent2_,
            action2
        );
    agentController2_->onActionExecuted(executed2);


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
