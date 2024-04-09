//
// Created by stefan on 08/04/24.
//

#include "NeighboursHelpers.h"
#include "Board.h"

namespace board {

    List<CellCoords *> NeighboursHelper::get(
            const CellCoords *cellCoords, const VisitedType *visited, bool emptyAllowed) const {
        List<CellCoords *> neighbours;
        fill(neighbours, cellCoords, visited, emptyAllowed);
        return neighbours;
    }

    void RedNeighboursHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                   const VisitedType *visited, bool emptyAllowed) const {
        auto row = cellCoords->row;
        auto num = cellCoords->num;
        if (cellCoords->direction != CellCoords::Direction::left &&
            cellCoords->direction != CellCoords::Direction::down) {
            addNeighbourAboveLeft(row, num, Cell::Type::red, neighbours, visited, emptyAllowed);
        }
        addNeighbourAbove(row, num, Cell::Type::red, neighbours, visited, emptyAllowed);
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
                                    const VisitedType *visited, bool emptyAllowed) const {
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
        addNeighbourAbove(row, num, Cell::Type::blue, neighbours, visited, emptyAllowed);
        if (cellCoords->direction == CellCoords::Direction::up ||
            cellCoords->direction == CellCoords::Direction::right) {
            addNeighbourAboveLeft(row, num, Cell::Type::blue, neighbours, visited, emptyAllowed);
        }
    }

    void EmptyNeighbourHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                    const NeighboursHelper::VisitedType *visited, bool emptyAllowed) const {
        auto row = cellCoords->row;
        auto num = cellCoords->num;
        addNeighbourAboveLeft(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourAbove(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourRight(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourBelowRight(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourBelow(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourLeft(row, num, mColor, neighbours, visited, emptyAllowed);
    }

    void NeighboursHelper::addNeighbourBelow(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                                             const VisitedType *visited, bool emptyAllowed) const {
        if (row < mBoard.size() - 1) {
            addToListIfNotVisited(row + 1, num, CellCoords::Direction::down, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourAbove(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                                             const VisitedType *visited, bool emptyAllowed) const {
        if (row > 0) {
            addToListIfNotVisited(row - 1, num, CellCoords::Direction::up, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void
    NeighboursHelper::addNeighbourAboveLeft(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                                            const VisitedType *visited, bool emptyAllowed) const {
        if (row > 0 && num > 0) {
            addToListIfNotVisited(row - 1, num - 1, CellCoords::Direction::upLeft, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void
    NeighboursHelper::addNeighbourBelowRight(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                                             const VisitedType *visited, bool emptyAllowed) const {
        if (row < mBoard.size() - 1 && num < mBoard.size() - 1) {
            addToListIfNotVisited(row + 1, num + 1, CellCoords::Direction::downRight, row, num, neighbours, visited,
                                  color,
                                  emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourLeft(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                                            const VisitedType *visited, bool emptyAllowed) const {
        if (num > 0) {
            addToListIfNotVisited(row, num - 1, CellCoords::Direction::left, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourRight(size_t row, size_t num, Cell::Type color, List<CellCoords *> &neighbours,
                                             const VisitedType *visited, bool emptyAllowed) const {
        if (num < mBoard.size() - 1) {
            addToListIfNotVisited(row, num + 1, CellCoords::Direction::right, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void NeighboursHelper::addToListIfNotVisited(
            size_t row, size_t num, CellCoords::Direction direction, size_t parentRow, size_t parentNum,
            List<CellCoords *> &list, const VisitedType *visited, Cell::Type color, bool emptyAllowed) const {
        if (!(*visited)[row][num]) {
            if (mBoard.getType(row, num) == color || (emptyAllowed && mBoard.getType(row, num) == Cell::Type::empty)) {
                list.pushBack(new CellCoords{row, num, direction});
            }
        }
    }

    void EmptyNeighbourHelper::addToListIfNotVisited(
            size_t row, size_t num, CellCoords::Direction direction, size_t parentRow, size_t parentNum,
            List<CellCoords *> &list, const NeighboursHelper::VisitedType *visited, Cell::Type color,
            bool emptyAllowed) const {
        if (mDistanceUpdater.shouldVisit(row, num, parentRow, parentNum)) {
            if (mBoard.getType(row, num) == Cell::Type::empty ||
                (mBoard.getType(parentRow, parentNum) == color && mBoard.getType(row, num) == color)) {
                list.pushBack(new CellCoords{row, num, direction});
                mDistanceUpdater.updateDistance(row, num, parentRow, parentNum);
            }
        }
    }

    void DistanceUpdater::updateDistance(size_t row, size_t num, size_t parentRow, size_t parentNum) const {
        if (mBoard.getType(row, num) != Cell::Type::empty) {
            mDistances[row][num] = 0;
            return;
        }
        size_t distance = mDistances[parentRow][parentNum] + 1;
        if (distance < mDistances[row][num])
            mDistances[row][num] = distance;
    }

    bool DistanceUpdater::shouldVisit(size_t row, size_t num, size_t parentRow, size_t parentNum) const {
        return mDistances[parentRow][parentNum] + 1 < mDistances[row][num];
    }
}
