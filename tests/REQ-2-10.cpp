#include <gtest/gtest.h>
#include "game/game_engine.h"
#include "agent/dll_agent_adapter.h"
#include <iostream>

DllAgentAdapter agent1;
DllAgentAdapter agent2;

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
}

TEST_F(GameEngineTest, F002) {
    Map MAP = engine.getMap();
    ASSERT_EQ(MAP.getCell(0, 0), CellState::Wall);
    std::cout << "Requirement 2 passed!" << std::endl;
    
}