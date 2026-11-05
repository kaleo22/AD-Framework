#pragma once

#include <cstdint>

#include "game/game_state.h"
#include "game/packed_cells.h"


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
        return carriedFlag_ != FlagId::None;
    }

    FlagId getCarriedFlag() const
    {
        return carriedFlag_;
    }

    void setCarriedFlag(FlagId flag)
    {
        carriedFlag_ = flag;
    }

private:
    uint32_t id_ = 0;
    Position position_{0, 0};
    FlagId carriedFlag_ = FlagId::None;
};


class AgentObservation
{
public:
    static constexpr int VISION_RADIUS = 2;

    static constexpr uint32_t VISION_SIZE =
        VISION_RADIUS * 2 + 1;

    static constexpr uint32_t VISIBLE_CELL_COUNT =
        VISION_SIZE * VISION_SIZE;

    static constexpr uint32_t CENTER_INDEX = VISIBLE_CELL_COUNT / 2;
    static_assert(VISION_SIZE == VIEW_SIZE, "Internal vision and DLL view must have the same size");

    explicit AgentObservation(FlagId ownFlag = FlagId::None)
        : ownFlag_(ownFlag)
    {
    }

    FlagId getOwnFlag() const
    {
        return ownFlag_;
    }

    void setVisibleCell(uint32_t index, CellState state)
    {
        vision_.set(index, state);
    }

    CellState getVisibleCell(uint32_t index) const
    {
        return vision_.get(index);
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


    FlagId getCarriedFlag() const
    {
        return describeCell(getVisibleCell(CENTER_INDEX)).carriedFlag;
    }

    bool isCarryingFlag() const
    {
        return getCarriedFlag() != FlagId::None;
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
    PackedCells<VISIBLE_CELL_COUNT> vision_;
    FlagId ownFlag_;
    uint32_t round_ = 0;
    WorldSnapshot apiSnapshot_{};
};
