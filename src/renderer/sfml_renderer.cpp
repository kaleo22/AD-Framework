#include "renderer/sfml_renderer.h"

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

void SFMLRenderer::render(
    const GameState& state,
    const Map& map)
{
    window.clear(sf::Color(25, 28, 35));

    const float tileSize = 60.0f;

    const float boardWidth =
        state.width * tileSize;

    const float boardHeight =
        state.height * tileSize;

    const float startX =
        (1280.0f - boardWidth) / 2.0f;

    const float startY =
        (800.0f - boardHeight) / 2.0f;

    for (int y = 0; y < state.height; ++y)
    {
        for (int x = 0; x < state.width; ++x)
        {
            const Cell cell = map.getCell(x, y);

            sf::RectangleShape tile(
                {
                    tileSize - 2.0f,
                    tileSize - 2.0f
                }
            );

            tile.setPosition(
                {
                    startX + x * tileSize,
                    startY + y * tileSize
                }
            );

            switch (cell.getTerrain())
            {
                case Cell::Terrain::Empty:
                {
                    tile.setFillColor(
                        sf::Color(45, 50, 60)
                    );
                    break;
                }

                case Cell::Terrain::Wall:
                {
                    tile.setFillColor(
                        sf::Color(100, 105, 115)
                    );
                    break;
                }

                case Cell::Terrain::Start:
                {
                    tile.setFillColor(
                        sf::Color(55, 100, 75)
                    );
                    break;
                }

                case Cell::Terrain::Goal:
                {
                    tile.setFillColor(
                        sf::Color(130, 110, 50)
                    );
                    break;
                }
            }

            window.draw(tile);

            if (cell.getContent() == Cell::Content::Flag)
            {
                sf::CircleShape flag(
                    tileSize * 0.15f
                );

                flag.setFillColor(
                    sf::Color(240, 200, 50)
                );

                flag.setPosition(
                    {
                        startX
                            + x * tileSize
                            + tileSize * 0.35f,

                        startY
                            + y * tileSize
                            + tileSize * 0.35f
                    }
                );

                window.draw(flag);
            }
        }
    }

    sf::CircleShape player1(
        tileSize * 0.30f
    );

    player1.setFillColor(
        sf::Color(70, 140, 255)
    );

    player1.setPosition(
        {
            startX
                + state.player1.x * tileSize
                + tileSize * 0.2f,

            startY
                + state.player1.y * tileSize
                + tileSize * 0.2f
        }
    );

    window.draw(player1);

    sf::CircleShape player2(
        tileSize * 0.30f
    );

    player2.setFillColor(
        sf::Color(230, 80, 90)
    );

    player2.setPosition(
        {
            startX
                + state.player2.x * tileSize
                + tileSize * 0.2f,

            startY
                + state.player2.y * tileSize
                + tileSize * 0.2f
        }
    );

    window.draw(player2);

    window.display();
}

void SFMLRenderer::renderAgentView(
    const AgentObservation& observation)
{
    if (!agentViewWindow.isOpen())
        return;

    agentViewWindow.clear(
        sf::Color(25, 28, 35)
    );

    const float tileSize = 90.0f;

    const float boardSize =
        AgentObservation::VISION_SIZE * tileSize;

    const sf::Vector2u windowSize =
        agentViewWindow.getSize();

    const float startX =
        (static_cast<float>(windowSize.x) - boardSize) / 2.0f;

    const float startY =
        (static_cast<float>(windowSize.y) - boardSize) / 2.0f;

    for (
        uint32_t index = 0;
        index < AgentObservation::VISIBLE_CELL_COUNT;
        ++index
    )
    {
        const uint32_t x =
            index % AgentObservation::VISION_SIZE;

        const uint32_t y =
            index / AgentObservation::VISION_SIZE;

        const uint8_t encoded =
            observation.getVisibleCell(index);

        const Cell cell =
            Cell::decode(encoded);

        sf::RectangleShape tile(
            {
                tileSize - 2.0f,
                tileSize - 2.0f
            }
        );

        tile.setPosition(
            {
                startX + x * tileSize,
                startY + y * tileSize
            }
        );

        switch (cell.getTerrain())
        {
            case Cell::Terrain::Empty:
                tile.setFillColor(
                    sf::Color(45, 50, 60)
                );
                break;

            case Cell::Terrain::Wall:
                tile.setFillColor(
                    sf::Color(100, 105, 115)
                );
                break;

            case Cell::Terrain::Start:
                tile.setFillColor(
                    sf::Color(55, 100, 75)
                );
                break;

            case Cell::Terrain::Goal:
                tile.setFillColor(
                    sf::Color(130, 110, 50)
                );
                break;
        }

        agentViewWindow.draw(tile);

        if (
            cell.getContent()
            == Cell::Content::Flag
        )
        {
            sf::CircleShape flag(
                tileSize * 0.15f
            );

            flag.setFillColor(
                sf::Color(240, 200, 50)
            );

            flag.setPosition(
                {
                    startX
                        + x * tileSize
                        + tileSize * 0.35f,

                    startY
                        + y * tileSize
                        + tileSize * 0.35f
                }
            );

            agentViewWindow.draw(flag);
        }

        if (
            cell.getContent()
                == Cell::Content::Agent ||
            cell.getContent()
                == Cell::Content::AgentWithFlag
        )
        {
            sf::CircleShape agent(
                tileSize * 0.30f
            );

            agent.setFillColor(
                sf::Color(70, 140, 255)
            );

            agent.setPosition(
                {
                    startX
                        + x * tileSize
                        + tileSize * 0.2f,

                    startY
                        + y * tileSize
                        + tileSize * 0.2f
                }
            );

            agentViewWindow.draw(agent);
        }
    }

    agentViewWindow.display();
}