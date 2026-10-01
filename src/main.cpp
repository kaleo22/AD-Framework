#include "game/game_engine.h"
#include "agent/console_agent.h"
#include "agent/wait_agent.h"
#include "renderer/sfml_renderer.h"

int main()
{
    GameEngine gameEngine;

    ConsoleAgent player1;
    WaitAgent player2;

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