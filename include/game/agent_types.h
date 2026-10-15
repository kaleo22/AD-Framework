#pragma once

#include <array>
#include <cstdint>

#include "game/game_state.h"


class GameAgent
{
public:
    GameAgent() = default;

    GameAgent(
        uint32_t id,
        Position position)
        : id_(id),
          position_(position)
    {
    }

    uint32_t getId() const
    {
        return id_;
    }

    const Position& getPosition() const
    {
        return position_;
    }

    void setPosition(Position position)
    {
        position_ = position;
    }

    bool isCarryingFlag() const
    {
        return carryingFlag_;
    }

    void setCarryingFlag(bool carryingFlag)
    {
        carryingFlag_ = carryingFlag;
    }

private:
    uint32_t id_ = 0;
    Position position_{0, 0};
    bool carryingFlag_ = false;
};


class AgentObservation
{
public:
    static constexpr int VISION_RADIUS = 2;

    static constexpr uint32_t VISION_SIZE =
        VISION_RADIUS * 2 + 1;

    static constexpr uint32_t VISIBLE_CELL_COUNT =
        VISION_SIZE * VISION_SIZE;

    static constexpr uint32_t CELLS_PER_WORD = 8;

    static constexpr uint32_t WORD_COUNT =
        (VISIBLE_CELL_COUNT + CELLS_PER_WORD - 1)
        / CELLS_PER_WORD;


    void setVisibleCell(
        uint32_t index,
        uint8_t encodedCell)
    {
        if (index >= VISIBLE_CELL_COUNT)
            return;

        const uint32_t wordIndex =
            index / CELLS_PER_WORD;

        const uint32_t cellIndex =
            index % CELLS_PER_WORD;

        const uint32_t shift =
            cellIndex * 4;

        const uint32_t mask =
            0x0F << shift;

        vision_[wordIndex] &= ~mask;

        vision_[wordIndex] |=
            (static_cast<uint32_t>(encodedCell) & 0x0F)
            << shift;
    }


    uint8_t getVisibleCell(
        uint32_t index) const
    {
        if (index >= VISIBLE_CELL_COUNT)
            return 0;

        const uint32_t wordIndex =
            index / CELLS_PER_WORD;

        const uint32_t cellIndex =
            index % CELLS_PER_WORD;

        const uint32_t shift =
            cellIndex * 4;

        return static_cast<uint8_t>(
            (vision_[wordIndex] >> shift) & 0x0F
        );
    }


    const std::array<uint32_t, WORD_COUNT>&
    getVision() const
    {
        return vision_;
    }

    void setApiCell(uint32_t index, uint32_t value)
    {
        if (index >= VISIBLE_CELL_COUNT)
            return;

        const uint32_t y = index / VISION_SIZE;
        const uint32_t x = index % VISION_SIZE;

        apiSnapshot_.cells[y][x] = value;
    }

    const WorldSnapshot& getApiSnapshot() const
    {
        return apiSnapshot_;
    }


    bool isCarryingFlag() const
    {
        return carryingFlag_;
    }

    void setCarryingFlag(bool carryingFlag)
    {
        carryingFlag_ = carryingFlag;
    }


    uint32_t getRound() const
    {
        return round_;
    }

    void setRound(uint32_t round)
    {
        round_ = round;
    }

private:
    std::array<uint32_t, WORD_COUNT> vision_{};

    bool carryingFlag_ = false;
    uint32_t round_ = 0;
    WorldSnapshot apiSnapshot_{};
};