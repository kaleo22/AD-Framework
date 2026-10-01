#pragma once

#include <SFML/Graphics.hpp>

#include "renderer_interface.h"

class SFMLRenderer : public IRenderer
{
public:
    bool initialize() override;

    bool isOpen() const override;

    void processEvents() override;

    void render(
        const GameState& state,
        const Map& map
    ) override;

private:
    sf::RenderWindow window;
};