#pragma once

#include <SFML/Graphics.hpp>

#include "renderer_interface.h"

class SFMLRenderer : public IRenderer
{
public:
    bool initialize() override;

    bool isOpen() const override;

    void processEvents() override;

    void renderAgentView(
        const AgentObservation& observation
    );

    void render(
        const GameState& state,
        const Map& map
    ) override;

private:
    sf::RenderWindow window;
    sf::RenderWindow agentViewWindow;
};

void renderAgentView(
    const AgentObservation& observation
);