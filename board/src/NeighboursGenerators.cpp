//
// Created by stefan on 08/04/24.
//

#include "NeighboursGenerators.h"
#include "Board.h"

using data_structures::List;

namespace board {

    List <CellCoords> NeighboursGenerator::get(
            const CellCoords &cellCoords, bool emptyAllowed) const {
        List<CellCoords> neighbours;
        fill(neighbours, cellCoords, emptyAllowed);
        return neighbours;
    }

    void RedNeighboursGenerator::fill(List <CellCoords> &neighbours, CellCoords cellCoords,
                                      bool emptyAllowed) const {
        auto row = cellCoords.row;
        auto num = cellCoords.num;
        auto *cell = mBoard.getCell(cellCoords);
        if (cellCoords.direction != CellCoords::Direction::left)
            addNeighbourAbove(row, num, cell, CellType::red, neighbours, emptyAllowed);
        addNeighboursOnRight(row, num, cell, CellType::red, neighbours, emptyAllowed, true);
        addNeighbourBelow(row, num, cell, CellType::red, neighbours, emptyAllowed);
        addNeighboursOnLeft(row, num, cell, CellType::red, neighbours, emptyAllowed, false);
        if (cellCoords.direction == CellCoords::Direction::left)
            addNeighbourAbove(row, num, cell, CellType::red, neighbours, emptyAllowed);
    }

    void BlueNeighboursGenerator::fill(List <CellCoords> &neighbours, CellCoords cellCoords,
                                       bool emptyAllowed) const {
        auto row = cellCoords.row;
        auto num = cellCoords.num;
        auto *cell = mBoard.getCell(cellCoords);
        if (cellCoords.direction != CellCoords::Direction::right)
            addNeighbourAbove(row, num, cell, CellType::blue, neighbours, emptyAllowed);
        addNeighboursOnLeft(row, num, cell, CellType::blue, neighbours, emptyAllowed, true);
        addNeighbourBelow(row, num, cell, CellType::blue, neighbours, emptyAllowed);
        addNeighboursOnRight(row, num, cell, CellType::blue, neighbours, emptyAllowed, false);
        if (cellCoords.direction == CellCoords::Direction::right)
            addNeighbourAbove(row, num, cell, CellType::blue, neighbours, emptyAllowed);
    }

    void NeighboursGenerator::addNeighbourBelow(
            size_t row, size_t num, Cell *cell, NeighboursGenerator::CellType color,
            List <CellCoords> &neighbours, bool emptyAllowed) const {
        if (row < mBoard.rows() - 2) {
            if (row == mBoard.getSize() - 2) {
                addToListIfNotVisited(row + 2, num, neighbours, cell, CellCoords::Direction::down, color,
                                      emptyAllowed);
            } else {
                if (row < mBoard.getSize() - 1) {
                    addToListIfNotVisited(row + 2, num + 1, neighbours, cell, CellCoords::Direction::down, color,
                                          emptyAllowed);
                } else {
                    if (num > 0 && num < mBoard.cells(row) - 1)
                        addToListIfNotVisited(row + 2, num - 1, neighbours, cell, CellCoords::Direction::down,
                                              color, emptyAllowed);
                }
            }
        }
    }

    void NeighboursGenerator::addNeighbourAbove(
            size_t row, size_t num, Cell *cell, NeighboursGenerator::CellType color,
            List <CellCoords> &neighbours, bool emptyAllowed) const {
        if (row > 1) {
            if (row == mBoard.getSize()) {
                addToListIfNotVisited(row - 2, num, neighbours, cell, CellCoords::Direction::up, color,
                                      emptyAllowed);
            } else {
                if (row < mBoard.getSize()) {
                    if (num > 0 && num < mBoard.cells(row) - 1)
                        addToListIfNotVisited(row - 2, num - 1, neighbours, cell, CellCoords::Direction::up,
                                              color, emptyAllowed);
                } else {
                    addToListIfNotVisited(row - 2, num + 1, neighbours, cell, CellCoords::Direction::up, color,
                                          emptyAllowed);
                }
            }
        }
    }

    void NeighboursGenerator::addNeighboursOnLeft(
            size_t row, size_t num, Cell *cell, NeighboursGenerator::CellType color,
            List <CellCoords> &neighbours, bool emptyAllowed, bool topFirst) const {
        if (row == mBoard.getSize() - 1) {
            if (num > 0) {
                if (!topFirst)
                    addToListIfNotVisited(row + 1, num - 1, neighbours, cell, CellCoords::Direction::left, color,
                                          emptyAllowed);
                addToListIfNotVisited(row - 1, num - 1, neighbours, cell, CellCoords::Direction::left, color,
                                      emptyAllowed);
                if (topFirst)
                    addToListIfNotVisited(row + 1, num - 1, neighbours, cell, CellCoords::Direction::left, color,
                                          emptyAllowed);
            }
        } else {
            if (row < mBoard.getSize()) {
                if (!topFirst)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::left, color,
                                          emptyAllowed);
                if (num > 0)
                    addToListIfNotVisited(row - 1, num - 1, neighbours, cell, CellCoords::Direction::left, color,
                                          emptyAllowed);
                if (topFirst)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::left, color,
                                          emptyAllowed);
            } else {
                if (!topFirst && num > 0)
                    addToListIfNotVisited(row + 1, num - 1, neighbours, cell, CellCoords::Direction::left, color,
                                          emptyAllowed);
                addToListIfNotVisited(row - 1, num, neighbours, cell, CellCoords::Direction::left, color,
                                      emptyAllowed);
                if (topFirst && num > 0)
                    addToListIfNotVisited(row + 1, num - 1, neighbours, cell, CellCoords::Direction::left, color,
                                          emptyAllowed);
            }
        }
    }

    void NeighboursGenerator::addNeighboursOnRight(
            size_t row, size_t num, Cell *cell, NeighboursGenerator::CellType color,
            List <CellCoords> &neighbours, bool emptyAllowed, bool topFirst) const {
        if (row == mBoard.getSize() - 1) {
            if (num < mBoard.getSize() - 1) {
                if (!topFirst)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::right, color,
                                          emptyAllowed);
                addToListIfNotVisited(row - 1, num, neighbours, cell, CellCoords::Direction::right, color,
                                      emptyAllowed);
                if (topFirst)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::right, color,
                                          emptyAllowed);
            }
        } else {
            if (row < mBoard.getSize()) {
                if (!topFirst)
                    addToListIfNotVisited(row + 1, num + 1, neighbours, cell, CellCoords::Direction::right,
                                          color, emptyAllowed);
                if (row > 0 && num < mBoard.cells(row) - 1)
                    addToListIfNotVisited(row - 1, num, neighbours, cell, CellCoords::Direction::right, color,
                                          emptyAllowed);
                if (topFirst)
                    addToListIfNotVisited(row + 1, num + 1, neighbours, cell, CellCoords::Direction::right,
                                          color, emptyAllowed);
            } else {
                if (!topFirst && row < mBoard.rows() - 1 && num < mBoard.cells(row) - 1)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::right, color,
                                          emptyAllowed);
                addToListIfNotVisited(row - 1, num + 1, neighbours, cell, CellCoords::Direction::right, color,
                                      emptyAllowed);
                if (topFirst && row < mBoard.rows() - 1 && num < mBoard.cells(row) - 1)
                    addToListIfNotVisited(row + 1, num, neighbours, cell, CellCoords::Direction::right, color,
                                          emptyAllowed);
            }
        }
    }

    void NeighboursGenerator::addToListIfNotVisited(
            size_t row, size_t num, List <CellCoords> &list, Cell *parent,
            CellCoords::Direction direction, NeighboursGenerator::CellType color, bool emptyAllowed) const {
        if (!mBoard.visited(row, num)) {
            auto cell = mBoard.getCell(row, num);
            if (cell->getType() == color || (emptyAllowed && cell->getType() == Cell::Type::empty))
                list.pushBack({row, num, parent, direction});
        }
    }
}
