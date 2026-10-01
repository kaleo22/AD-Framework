#pragma once

#include <array>
#include <cstdint>

#include "game/game_state.h"
#include "game/agent_types.h"
#include "framework/plugin_manager.h"
#include "agent/agent_interface.h"


class Cell
{
public:
    enum class Terrain : uint8_t
    {
        Empty = 0,
        Wall = 1,
        Start = 2,
        Goal = 3
    };

    enum class Content : uint8_t
    {
        None = 0,
        Flag = 1,
        Agent = 2,
        AgentWithFlag = 3
    };

    Cell() = default;

    Cell(Terrain terrain, Content content)
        : terrain_(terrain),
          content_(content)
    {
    }

    uint8_t encode() const
    {
        return
            (static_cast<uint8_t>(terrain_) & 0x03)
            |
            ((static_cast<uint8_t>(content_) & 0x03) << 2);
    }

    static Cell decode(uint8_t encoded)
    {
        const Terrain terrain =
            static_cast<Terrain>(
                encoded & 0x03
            );

        const Content content =
            static_cast<Content>(
                (encoded >> 2) & 0x03
            );

        return Cell(
            terrain,
            content
        );
    }

    Terrain getTerrain() const
    {
        return terrain_;
    }

    Content getContent() const
    {
        return content_;
    }

private:
    Terrain terrain_ = Terrain::Empty;
    Content content_ = Content::None;
};


class Map
{
public:
    static constexpr uint32_t width_ = 16;
    static constexpr uint32_t height_ = 10;

    static constexpr uint32_t CELL_COUNT =
        width_ * height_;

    Cell getCell(
        uint32_t x,
        uint32_t y
    ) const
    {
        const uint32_t index =
            y * width_ + x;

        const uint32_t byteIndex =
            index / 2;

        const bool upperNibble =
            index % 2;

        uint8_t encoded;

        if (upperNibble)
        {
            encoded =
                (data_[byteIndex] >> 4)
                & 0x0F;
        }
        else
        {
            encoded =
                data_[byteIndex]
                & 0x0F;
        }

        return Cell::decode(encoded);
    }

    void setCell(
        uint32_t x,
        uint32_t y,
        const Cell& cell
    )
    {
        const uint32_t index =
            y * width_ + x;

        const uint32_t byteIndex =
            index / 2;

        const bool upperNibble =
            index % 2;

        const uint8_t encoded =
            cell.encode() & 0x0F;

        if (upperNibble)
        {
            data_[byteIndex] &= 0x0F;

            data_[byteIndex] |=
                static_cast<uint8_t>(
                    encoded << 4
                );
        }
        else
        {
            data_[byteIndex] &= 0xF0;

            data_[byteIndex] |= encoded;
        }
    }

    void clear()
    {
        data_.fill(0);
    }

private:
    std::array<
        uint8_t,
        (CELL_COUNT + 1) / 2
    > data_{};
};


// class IAgent
// {
// public:
//     virtual ~IAgent() = default;
//
//     virtual AgentAction getAction(
//         const AgentObservation& observation
//     ) = 0;
// };


class GameEngine
{
public:

    void setAgents(
        IAgent* agent1,
        IAgent* agent2
    );

    GameEngine();

    void update();

    GameStates getGameState() const;

    const GameState& getState() const;

    const Map& getMap() const;

    AgentObservation getAgent1Observation() const;

private:

    IAgent* agentController1_ = nullptr;
    IAgent* agentController2_ = nullptr;
    bool initialize();

    void initializeTestMap();

    void setState(
        GameStates newState
    );

    void updateRunning();

    void checkGameFinished();

    void shutdown();

    void executeAction(
        AgentInstance& agent,
        AgentAction action
    );

    bool tryMove(
        AgentInstance& agent,
        Position target
    );

    bool tryTakeFlag(
        AgentInstance& agent
    );

    bool tryDropFlag(
        AgentInstance& agent
    );

    bool isOccupied(
        Position position
    ) const;

    AgentObservation buildObservation(
        const AgentInstance& agent
    ) const;

    void syncGameState();

private:
    Map map_;

    PluginManager pluginManager_;

    AgentInstance agent1_{
        1,
        Position{2, 2}
    };

    AgentInstance agent2_{
        2,
        Position{13, 7}
    };

    GameState state;

    GameStates state_ =
        GameStates::Initializing;

    uint32_t currentRound_ = 0;
};