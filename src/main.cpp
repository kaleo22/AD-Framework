#include "game/game_engine.h"
#include "agent/dll_agent_adapter.h"
#include "renderer/sfml_renderer.h"

int main()
{
    GameEngine gameEngine;

    DllAgentAdapter player1;
    DllAgentAdapter player2;

    if (!player1.load(
            "simple_agent.dll",
            Map::width_,
            Map::height_
        ))
    {
        return 1;
    }

    if (!player2.load(
            "simple_agent.dll",
            Map::width_,
            Map::height_
        ))
    {
        return 1;
    }

    gameEngine.setAgents(
        &player1,
        &player2
    );

    SFMLRenderer renderer;

    if (!renderer.initialize())
        return 1;

    while (renderer.isOpen())
    {
        renderer.processEvents();

        gameEngine.update();

        renderer.render(
            gameEngine.getState(),
            gameEngine.getMap()
        );

        renderer.renderAgentView(
            gameEngine.getAgent1Observation()
        );
    }

    return 0;
}