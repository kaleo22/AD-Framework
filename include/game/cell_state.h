#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

enum class FlagId : std::uint8_t
{
    None,
    Team1,
    Team2
};

enum class CellState : std::uint8_t
{
    Ground                         = 0b00000,
    Wall                           = 0b00001,
    Unknown                        = 0b00010,
    Base                           = 0b00011,
    GroundFlag1                    = 0b00100,
    GroundFlag2                    = 0b00101,
    GroundAgent                    = 0b00110,
    GroundAgentOnFlag1             = 0b00111,
    GroundAgentOnFlag2             = 0b01000,
    GroundAgentCarryingFlag1       = 0b01001,
    GroundAgentCarryingFlag2       = 0b01010,
    GroundAgentCarryingFlag1OnFlag2 = 0b01011,
    GroundAgentCarryingFlag2OnFlag1 = 0b01100,
    BaseFlag1                      = 0b01101,
    BaseFlag2                      = 0b01110,
    BaseAgent                      = 0b01111,
    BaseAgentOnFlag1               = 0b10000,
    BaseAgentOnFlag2               = 0b10001,
    BaseAgentCarryingFlag1         = 0b10010,
    BaseAgentCarryingFlag2         = 0b10011,
    BaseAgentCarryingFlag1OnFlag2   = 0b10100,
    BaseAgentCarryingFlag2OnFlag1   = 0b10101
};

struct CellInfo
{
    CellState code;
    CellState surface;
    FlagId groundFlag = FlagId::None;
    bool occupied = false;
    FlagId carriedFlag = FlagId::None;
};

inline constexpr std::array<CellInfo, 22> CELL_STATES{{
    {CellState::Ground,                         CellState::Ground},
    {CellState::Wall,                           CellState::Wall},
    {CellState::Unknown,                        CellState::Unknown},
    {CellState::Base,                           CellState::Base},
    {CellState::GroundFlag1,                    CellState::Ground, FlagId::Team1},
    {CellState::GroundFlag2,                    CellState::Ground, FlagId::Team2},
    {CellState::GroundAgent,                    CellState::Ground, FlagId::None,  true},
    {CellState::GroundAgentOnFlag1,             CellState::Ground, FlagId::Team1, true},
    {CellState::GroundAgentOnFlag2,             CellState::Ground, FlagId::Team2, true},
    {CellState::GroundAgentCarryingFlag1,       CellState::Ground, FlagId::None,  true, FlagId::Team1},
    {CellState::GroundAgentCarryingFlag2,       CellState::Ground, FlagId::None,  true, FlagId::Team2},
    {CellState::GroundAgentCarryingFlag1OnFlag2, CellState::Ground, FlagId::Team2, true, FlagId::Team1},
    {CellState::GroundAgentCarryingFlag2OnFlag1, CellState::Ground, FlagId::Team1, true, FlagId::Team2},
    {CellState::BaseFlag1,                      CellState::Base,   FlagId::Team1},
    {CellState::BaseFlag2,                      CellState::Base,   FlagId::Team2},
    {CellState::BaseAgent,                      CellState::Base,   FlagId::None,  true},
    {CellState::BaseAgentOnFlag1,               CellState::Base,   FlagId::Team1, true},
    {CellState::BaseAgentOnFlag2,               CellState::Base,   FlagId::Team2, true},
    {CellState::BaseAgentCarryingFlag1,         CellState::Base,   FlagId::None,  true, FlagId::Team1},
    {CellState::BaseAgentCarryingFlag2,         CellState::Base,   FlagId::None,  true, FlagId::Team2},
    {CellState::BaseAgentCarryingFlag1OnFlag2,   CellState::Base,   FlagId::Team2, true, FlagId::Team1},
    {CellState::BaseAgentCarryingFlag2OnFlag1,   CellState::Base,   FlagId::Team1, true, FlagId::Team2}
}};

constexpr bool cellTableMatchesCodes()
{
    for (std::size_t index = 0; index < CELL_STATES.size(); ++index)
    {
        if (static_cast<std::size_t>(CELL_STATES[index].code) != index)
            return false;
    }
    return true;
}

static_assert(cellTableMatchesCodes(), "Cell table must follow CellState code order");

constexpr const CellInfo& describeCell(CellState state)
{
    const auto index = static_cast<std::size_t>(state);
    if (index < CELL_STATES.size())
        return CELL_STATES[index];

    return CELL_STATES[static_cast<std::size_t>(CellState::Unknown)];
}

constexpr CellState makeCellState(
    CellState surface,
    FlagId groundFlag,
    bool occupied = false,
    FlagId carriedFlag = FlagId::None
)
{
    for (const CellInfo& state : CELL_STATES)
    {
        if (state.surface == surface &&
            state.groundFlag == groundFlag &&
            state.occupied == occupied &&
            state.carriedFlag == carriedFlag)
        {
            return state.code;
        }
    }
    return CellState::Unknown;
}

constexpr CellState withAgent(CellState cell, FlagId carriedFlag)
{
    const CellInfo& info = describeCell(cell);
    return makeCellState(info.surface, info.groundFlag, true, carriedFlag);
}
