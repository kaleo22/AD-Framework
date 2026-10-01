#pragma once

#include <vector>
#include <cstddef>


enum class Celltype {
    Empty,
    Wall,
    Start,
    Goal,
    Flag,
    Enemy
};

//struct Cell {
  //  Celltype type = Celltype::Empty;
//};

// class Map {
// public:
//     Map(std::size_t width, std::size_t height);
//
//     bool isInside(int x, int y) const;
//     const Cell& getCell(int x, int y) const;
//     Cell& getCell(int x, int y);
//
//     std::size_t getWidth() const;
//     std::size_t getHeight() const;
//
// private:
//     std::size_t width;
//     std::size_t height;
//     std::vector<Cell> cells;
// };
