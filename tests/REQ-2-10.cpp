#include <gtest/gtest.h>
#include "game/game_engine.h"
#include "agent/dll_agent_adapter.h"
#include <iostream>
#include "framework/agent_loader.h"

DllAgentAdapter agent1;
DllAgentAdapter agent2;

class TestAgent : public IAgent
{
    public:
    AgentObservation observation;
    AgentAction action;

    AgentAction getAction(const AgentObservation& observe) override
    {
        observation = observe;
        return action;
    }
};

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

    void setPositions(Position position1, Position position2)
    {
        engine.agent1_.setPosition(position1);
        engine.agent2_.setPosition(position2);
        engine.syncGameState();
    }

    GameEngine engine;
    int success = !agent1.load(
    "C:/Users/leona/projects/AD-Framework/build/simple_agent.dll",
    Map::width_,
    Map::height_
    );

    int success2 = !agent2.load(
    "C:/Users/leona/projects/AD-Framework/build/simple_agent.dll",
    Map::width_,
    Map::height_
    );

    int suc = success + success2;

    void build_map(){

        for (int i = 0; i<16; i++){
            for (int j=0; j<10; j++){
                if (j == 0 || i == 0 || i == 15 || j == 9){
                    setCell(i, j, CellState::Wall);
                }
                else{
                    setCell(i, j, CellState::Ground);
                }
                }
        }
    }

};

TEST_F(GameEngineTest, F003_F004_F005AgentLoadingAndUnloadingWhileRuntime)
{
    RecordProperty("requirements", "REQ-F-003, REQ-F004, REQ-F-005");
    ASSERT_EQ(suc, 0);
    ASSERT_TRUE(agent1.unload(engine));
    ASSERT_EQ(engine.getGameState(), GameStates::WaitingForPlayers);

    engine.update();
    engine.update();
    engine.update();
    ASSERT_EQ(engine.getGameState(), GameStates::WaitingForPlayers);

    ASSERT_TRUE(agent2.unload(engine));
    ASSERT_EQ(engine.getGameState(), GameStates::WaitingForPlayers);

    ASSERT_TRUE(agent1.load(
        "C:/Users/leona/projects/AD-Framework/build/simple_agent.dll",
        Map::width_, Map::height_));
    ASSERT_TRUE(agent2.load(
        "C:/Users/leona/projects/AD-Framework/build/simple_agent.dll",
        Map::width_, Map::height_));
    engine.setAgents(&agent1, &agent2);
    engine.update();
    engine.update();
    ASSERT_EQ(engine.getGameState(), GameStates::Running);
}

TEST_F(GameEngineTest, F002) {
    Map MAP = engine.getMap();
    ASSERT_EQ(MAP.getCell(0, 0), CellState::Wall);
    
}

TEST_F(GameEngineTest, F006_RoundBasedGameLogic)
{
    RecordProperty("requirements", "REQ-F-006");
    const auto round = engine.getAgent1Observation().getRound();

    engine.update();

    ASSERT_EQ(engine.getAgent1Observation().getRound(), round + 1);

}

TEST_F(GameEngineTest, F007_BothAgentsSeeTheSameAndActOnIt)
{
    TestAgent test_a1;
    TestAgent test_a2;
    engine.setAgents(&test_a1, &test_a2);
    setPositions({4,3}, {6,3});

    test_a1.action = AgentAction::MoveLeft;
    test_a2.action = AgentAction::Wait;

    engine.update();

    const auto index = AgentObservation::CENTER_INDEX - 2;
    const auto cell = test_a2.observation.getVisibleCell(index);

    ASSERT_EQ(
        cell, CellState::GroundAgent
    );
}

