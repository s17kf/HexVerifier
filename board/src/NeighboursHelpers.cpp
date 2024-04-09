//
// Created by stefan on 08/04/24.
//

#include "NeighboursHelpers.h"
#include "Board.h"

namespace board {

    List<CellCoords *> NeighboursHelper::get(
            const CellCoords *cellCoords, const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        List<CellCoords *> neighbours;
        fill(neighbours, cellCoords, visited, emptyAllowed);
        return neighbours;
    }

    void RedNeighboursHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                   const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        auto row = cellCoords->row;
        auto num = cellCoords->num;
        if (cellCoords->direction != CellCoords::Direction::left &&
            cellCoords->direction != CellCoords::Direction::down) {
            addNeighbourAboveLeft(row, num, Cell::Type::red, neighbours, visited, emptyAllowed);
        }
        addNeighbourAbove(row, num, CellType::red, neighbours, visited, emptyAllowed);
        addNeighbourRight(row, num, Cell::Type::red, neighbours, visited, emptyAllowed);
        addNeighbourBelowRight(row, num, Cell::Type::red, neighbours, visited, emptyAllowed);
        addNeighbourBelow(row, num, Cell::Type::red, neighbours, visited, emptyAllowed);
        addNeighbourLeft(row, num, Cell::Type::red, neighbours, visited, emptyAllowed);
        if (cellCoords->direction == CellCoords::Direction::left ||
            cellCoords->direction == CellCoords::Direction::down) {
            addNeighbourAboveLeft(row, num, Cell::Type::red, neighbours, visited, emptyAllowed);
        }
    }

    void BlueNeighboursHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                    const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        auto row = cellCoords->row;
        auto num = cellCoords->num;
        if (cellCoords->direction != CellCoords::Direction::up &&
            cellCoords->direction != CellCoords::Direction::right) {
            addNeighbourAboveLeft(row, num, Cell::Type::blue, neighbours, visited, emptyAllowed);
        }
        addNeighbourLeft(row, num, Cell::Type::blue, neighbours, visited, emptyAllowed);
        addNeighbourBelow(row, num, Cell::Type::blue, neighbours, visited, emptyAllowed);
        addNeighbourBelowRight(row, num, Cell::Type::blue, neighbours, visited, emptyAllowed);
        addNeighbourRight(row, num, Cell::Type::blue, neighbours, visited, emptyAllowed);
        addNeighbourAbove(row, num, CellType::blue, neighbours, visited, emptyAllowed);
        if (cellCoords->direction == CellCoords::Direction::up ||
            cellCoords->direction == CellCoords::Direction::right) {
            addNeighbourAboveLeft(row, num, Cell::Type::blue, neighbours, visited, emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourBelow(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                                             const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        if (row < mBoard.size() - 1) {
            addToListIfNotVisited(row + 1, num, CellCoords::Direction::down, neighbours, visited, color, emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourAbove(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                                             const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        if (row > 0) {
            addToListIfNotVisited(row - 1, num, CellCoords::Direction::up, neighbours, visited, color, emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourAboveLeft(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                                                 const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        if (row > 0 && num > 0) {
            addToListIfNotVisited(row - 1, num - 1, CellCoords::Direction::upLeft, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void
    NeighboursHelper::addNeighbourBelowRight(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                                             const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        if (row < mBoard.size() - 1 && num < mBoard.size() - 1) {
            addToListIfNotVisited(row + 1, num + 1, CellCoords::Direction::downRight, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourLeft(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                                            const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        if (num > 0) {
            addToListIfNotVisited(row, num - 1, CellCoords::Direction::left, neighbours, visited, color, emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourRight(size_t row, size_t num, CellType color, List<CellCoords *> &neighbours,
                                             const Vector<Vector<bool>> &visited, bool emptyAllowed) const {
        if (num < mBoard.size() - 1) {
            addToListIfNotVisited(row, num + 1, CellCoords::Direction::right, neighbours, visited, color, emptyAllowed);
        }
    }

    void NeighboursHelper::addToListIfNotVisited(
            size_t row, size_t num, CellCoords::Direction direction, List<CellCoords *> &list,
            const Vector<Vector<bool>> &visited, NeighboursHelper::CellType color, bool emptyAllowed) const {
        if (!visited[row][num]) {
            if (mBoard.getType(row, num) == color || (emptyAllowed && mBoard.getType(row, num) == Cell::Type::empty))
                list.pushBack(new CellCoords{row, num, direction});
        }
    }
}
