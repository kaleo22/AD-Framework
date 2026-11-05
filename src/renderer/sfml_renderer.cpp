#include "renderer/sfml_renderer.h"

namespace
{
const sf::Color player1Color{70, 140, 255};
const sf::Color player2Color{230, 80, 90};

sf::Color tileColor(CellState surface)
{
    switch (surface)
    {
        case CellState::Ground: return sf::Color(45, 50, 60);
        case CellState::Wall: return sf::Color(100, 105, 115);
        case CellState::Base: return sf::Color(55, 100, 75);
        default: return sf::Color(20, 23, 29);
    }
}

sf::Color flagColor(FlagId flag)
{
    return flag == FlagId::Team1 ? sf::Color(135, 225, 255) : sf::Color(255, 180, 100);
}

void drawMarker(sf::RenderTarget& target, sf::Vector2f center, float radius, sf::Color color)
{
    sf::CircleShape marker(radius);
    marker.setOrigin({radius, radius});
    marker.setPosition(center);
    marker.setFillColor(color);
    target.draw(marker);
}

void drawCell(
    sf::RenderTarget& target,
    sf::Vector2f origin,
    float tileSize,
    CellState state,
    sf::Color agentColor
)
{
    const CellInfo& cell = describeCell(state);
    sf::RectangleShape tile({tileSize - 2.0f, tileSize - 2.0f});
    tile.setPosition(origin);
    tile.setFillColor(tileColor(cell.surface));
    target.draw(tile);

    const sf::Vector2f center = origin + sf::Vector2f(tileSize * 0.5f, tileSize * 0.5f);
    if (cell.occupied)
        drawMarker(target, center, tileSize * 0.30f, agentColor);

    if (cell.groundFlag != FlagId::None)
    {
        const sf::Vector2f groundMarker = cell.occupied
            ? origin + sf::Vector2f(tileSize * 0.18f, tileSize * 0.80f)
            : center;
        drawMarker(target, groundMarker, tileSize * 0.10f, flagColor(cell.groundFlag));
    }

    if (cell.carriedFlag != FlagId::None)
        drawMarker(target, center, tileSize * 0.10f, flagColor(cell.carriedFlag));
}
}

bool SFMLRenderer::initialize()
{
    window.create(
        sf::VideoMode({1280, 800}),
        "AD Framework"
    );

    agentViewWindow.create(
        sf::VideoMode({600, 600}),
        "Agent View"
    );

    window.setFramerateLimit(60);

    return window.isOpen();
}

bool SFMLRenderer::isOpen() const
{
    return window.isOpen();
}

void SFMLRenderer::processEvents()
{
    while (const auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
    }

    while (const auto event = agentViewWindow.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            agentViewWindow.close();
        }
    }
}

void SFMLRenderer::render(const GameState& state, const Map& map)
{
    window.clear(sf::Color(25, 28, 35));

    const float tileSize = 60.0f;
    const float boardWidth = state.width * tileSize;
    const float boardHeight = state.height * tileSize;
    const float startX = (1280.0f - boardWidth) / 2.0f;
    const float startY = (800.0f - boardHeight) / 2.0f;

    for (int y = 0; y < state.height; ++y)
    {
        for (int x = 0; x < state.width; ++x)
        {
            const Position position{x, y};
            CellState cell = map.getCell(x, y);
            sf::Color agentColor = player1Color;

            if (position == state.player1)
                cell = withAgent(cell, state.player1CarriedFlag);
            else if (position == state.player2)
            {
                cell = withAgent(cell, state.player2CarriedFlag);
                agentColor = player2Color;
            }

            drawCell(window, {startX + x * tileSize, startY + y * tileSize}, tileSize, cell, agentColor);
        }
    }

    window.display();
}

void SFMLRenderer::renderAgentView(const AgentObservation& observation)
{
    if (!agentViewWindow.isOpen())
        return;

    agentViewWindow.clear(sf::Color(25, 28, 35));

    const float tileSize = 90.0f;
    const float boardSize = AgentObservation::VISION_SIZE * tileSize;
    const sf::Vector2u windowSize = agentViewWindow.getSize();
    const float startX = (static_cast<float>(windowSize.x) - boardSize) / 2.0f;
    const float startY = (static_cast<float>(windowSize.y) - boardSize) / 2.0f;
    const bool isPlayer1 = observation.getOwnFlag() == FlagId::Team1;
    const sf::Color selfColor = isPlayer1 ? player1Color : player2Color;
    const sf::Color enemyColor = isPlayer1 ? player2Color : player1Color;

    for (uint32_t index = 0; index < AgentObservation::VISIBLE_CELL_COUNT; ++index)
    {
        const uint32_t x = index % AgentObservation::VISION_SIZE;
        const uint32_t y = index / AgentObservation::VISION_SIZE;
        const bool isSelf = index == AgentObservation::CENTER_INDEX;

        drawCell(
            agentViewWindow,
            {startX + x * tileSize, startY + y * tileSize},
            tileSize,
            observation.getVisibleCell(index),
            isSelf ? selfColor : enemyColor
        );
    }

    agentViewWindow.display();
}
