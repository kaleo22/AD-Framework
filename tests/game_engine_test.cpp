#include <gtest/gtest.h>

#include <vector>

#include "game/game_engine.h"

namespace
{
class ScriptedAgent : public IAgent
{
public:
    AgentAction firstAction = AgentAction::Wait;
    std::vector<AgentObservation> observations;
    std::vector<AgentAction> executedActions;

    AgentAction getAction(const AgentObservation& observation) override
    {
        observations.push_back(observation);
        return observations.size() == 1 ? firstAction : AgentAction::Wait;
    }

    void onActionExecuted(AgentAction action) override
    {
        executedActions.push_back(action);
    }
};
}

class GameEngineTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        engine.setAgents(&agent1, &agent2);
        engine.update();
        engine.update();
        engine.update();
        ASSERT_EQ(engine.getGameState(), GameStates::Running);
    }

    void setCell(int x, int y, CellState cell)
    {
        engine.map_.setCell(x, y, cell);
    }

    ScriptedAgent agent1;
    ScriptedAgent agent2;
    GameEngine engine;
};

TEST_F(GameEngineTest, F017_F018_F019_WallBlocksMoveAndNextObservationReportsFailure)
{
    RecordProperty("requirements", "REQ-F-017, REQ-F-018, REQ-F-019");

    ASSERT_EQ(engine.getState().player1, (Position{2, 2}));
    ASSERT_EQ(engine.getState().player2, (Position{13, 7}));
    setCell(3, 2, CellState::Wall);
    ASSERT_EQ(engine.getMap().getCell(3, 2), CellState::Wall);
    agent1.firstAction = AgentAction::MoveRight;

    engine.update();

    EXPECT_EQ(engine.getState().player1, (Position{2, 2}));
    ASSERT_EQ(agent1.executedActions.size(), 1u);
    EXPECT_EQ(agent1.executedActions[0], AgentAction::Wait);
    ASSERT_EQ(agent1.observations.size(), 1u);
    EXPECT_EQ(agent1.observations[0].getRound(), 0u);
    EXPECT_FALSE(agent1.observations[0].getLastActionResult().has_value());

    engine.update();

    ASSERT_EQ(agent1.observations.size(), 2u);
    const AgentObservation& nextObservation = agent1.observations[1];
    EXPECT_EQ(nextObservation.getRound(), 1u);
    ASSERT_TRUE(nextObservation.getLastActionResult().has_value());
    const AgentActionResult& result = *nextObservation.getLastActionResult();
    EXPECT_EQ(result.requestedAction, AgentAction::MoveRight);
    EXPECT_EQ(result.executedAction, AgentAction::Wait);
    EXPECT_FALSE(result.wasSuccessful());

    ASSERT_EQ(agent2.observations.size(), 2u);
    ASSERT_TRUE(agent2.observations[1].getLastActionResult().has_value());
    EXPECT_TRUE(agent2.observations[1].getLastActionResult()->wasSuccessful());
}

TEST_F(GameEngineTest, F019_FreeTargetAllowsMoveAndNextObservationReportsSuccess)
{
    RecordProperty("requirements", "REQ-F-019");
    setCell(3, 2, CellState::Ground);
    agent1.firstAction = AgentAction::MoveRight;

    engine.update();

    EXPECT_EQ(engine.getState().player1, (Position{3, 2}));
    ASSERT_EQ(agent1.executedActions.size(), 1u);
    EXPECT_EQ(agent1.executedActions[0], AgentAction::MoveRight);

    engine.update();

    ASSERT_EQ(agent1.observations.size(), 2u);
    EXPECT_EQ(agent1.observations[1].getRound(), 1u);
    const auto& result = agent1.observations[1].getLastActionResult();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->requestedAction, AgentAction::MoveRight);
    EXPECT_EQ(result->executedAction, AgentAction::MoveRight);
    EXPECT_TRUE(result->wasSuccessful());
}

TEST_F(GameEngineTest, F019_DeliberateWaitIsSuccessfulAndReplacesPreviousFailure)
{
    RecordProperty("requirements", "REQ-F-019");
    setCell(3, 2, CellState::Wall);
    agent1.firstAction = AgentAction::MoveRight;

    engine.update();
    engine.update();
    engine.update();

    ASSERT_EQ(agent1.observations.size(), 3u);
    ASSERT_TRUE(agent1.observations[1].getLastActionResult().has_value());
    EXPECT_FALSE(agent1.observations[1].getLastActionResult()->wasSuccessful());
    EXPECT_EQ(engine.getState().player1, (Position{2, 2}));
    EXPECT_EQ(agent1.observations[2].getRound(), 2u);
    const auto& result = agent1.observations[2].getLastActionResult();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->requestedAction, AgentAction::Wait);
    EXPECT_EQ(result->executedAction, AgentAction::Wait);
    EXPECT_TRUE(result->wasSuccessful());
}
