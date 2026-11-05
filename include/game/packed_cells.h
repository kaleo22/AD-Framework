#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "game/cell_state.h"

template<std::size_t CellCount>
class PackedCells
{
public:
    static constexpr std::size_t BITS_PER_CELL = 5;
    static constexpr std::size_t BYTE_COUNT = (CellCount * BITS_PER_CELL + 7) / 8;

    CellState get(std::size_t index) const
    {
        if (index >= CellCount)
            return CellState::Unknown;

        const std::size_t bitIndex = index * BITS_PER_CELL;
        const std::size_t byteIndex = bitIndex / 8;
        const std::size_t shift = bitIndex % 8;
        const bool crossesByte = shift + BITS_PER_CELL > 8;
        const std::uint16_t window = readWindow(byteIndex, crossesByte);

        return static_cast<CellState>((window >> shift) & CELL_MASK);
    }

    void set(std::size_t index, CellState state)
    {
        if (index >= CellCount)
            return;

        const std::size_t bitIndex = index * BITS_PER_CELL;
        const std::size_t byteIndex = bitIndex / 8;
        const std::size_t shift = bitIndex % 8;
        const bool crossesByte = shift + BITS_PER_CELL > 8;

        std::uint16_t window = readWindow(byteIndex, crossesByte);
        const auto writeMask = static_cast<std::uint16_t>(CELL_MASK << shift);
        const auto code = static_cast<std::uint16_t>(state) & CELL_MASK;
        window = static_cast<std::uint16_t>((window & ~writeMask) | (code << shift));

        bytes_[byteIndex] = static_cast<std::uint8_t>(window);
        if (crossesByte)
            bytes_[byteIndex + 1] = static_cast<std::uint8_t>(window >> 8);
    }

    void clear()
    {
        bytes_.fill(0);
    }

private:
    static constexpr std::uint16_t CELL_MASK = 0b11111;
    static_assert(CELL_STATES.size() <= (1u << BITS_PER_CELL), "Too many cell states");
    static_assert(static_cast<std::uint8_t>(CellState::Ground) == 0, "Ground must be zero");

    std::uint16_t readWindow(std::size_t byteIndex, bool crossesByte) const
    {
        std::uint16_t window = bytes_[byteIndex];
        if (crossesByte)
        {
            window |= static_cast<std::uint16_t>(
                static_cast<std::uint16_t>(bytes_[byteIndex + 1]) << 8
            );
        }
        return window;
    }

    std::array<std::uint8_t, BYTE_COUNT> bytes_{};
};
