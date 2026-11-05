#pragma once

#include <cstdint>

#include "game/packed_cells.h"

class Map
{
public:
    static constexpr std::uint32_t width_ = 16;
    static constexpr std::uint32_t height_ = 10;
    static constexpr std::uint32_t CELL_COUNT = width_ * height_;

    CellState getCell(int x, int y) const
    {
        if (!contains(x, y))
            return CellState::Unknown;

        return cells_.get(indexOf(x, y));
    }

    void setCell(int x, int y, CellState state)
    {
        if (contains(x, y))
            cells_.set(indexOf(x, y), state);
    }

    void clear()
    {
        cells_.clear();
    }

private:
    static constexpr bool contains(int x, int y)
    {
        return x >= 0 && y >= 0 &&
            x < static_cast<int>(width_) && y < static_cast<int>(height_);
    }

    static constexpr std::size_t indexOf(int x, int y)
    {
        return static_cast<std::size_t>(y) * width_ + static_cast<std::size_t>(x);
    }

    PackedCells<CELL_COUNT> cells_;
};
