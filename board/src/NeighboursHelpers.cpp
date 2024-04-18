//
// Created by stefan on 08/04/24.
//

#include "NeighboursHelpers.h"
#include "Board.h"

using data_structures::List;

namespace board {

    void RedNeighboursHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                   const VisitedType *visited, bool emptyAllowed) const {
        const auto &row = cellCoords->row;
        const auto &num = cellCoords->num;
        if (cellCoords->direction != CellCoords::Direction::left &&
            cellCoords->direction != CellCoords::Direction::down) {
            addNeighbourAboveLeft(row, num, Color::red, neighbours, visited, emptyAllowed);
        }
        addNeighbourAbove(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourRight(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourBelowRight(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourBelow(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourLeft(row, num, Color::red, neighbours, visited, emptyAllowed);
        if (cellCoords->direction == CellCoords::Direction::left ||
            cellCoords->direction == CellCoords::Direction::down) {
            addNeighbourAboveLeft(row, num, Color::red, neighbours, visited, emptyAllowed);
        }
    }

    void BlueNeighboursHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                    const VisitedType *visited, bool emptyAllowed) const {
        const auto &row = cellCoords->row;
        const auto &num = cellCoords->num;
        if (cellCoords->direction != CellCoords::Direction::up &&
            cellCoords->direction != CellCoords::Direction::right) {
            addNeighbourAboveLeft(row, num, Color::blue, neighbours, visited, emptyAllowed);
        }
        addNeighbourLeft(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourBelow(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourBelowRight(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourRight(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourAbove(row, num, Color::blue, neighbours, visited, emptyAllowed);
        if (cellCoords->direction == CellCoords::Direction::up ||
            cellCoords->direction == CellCoords::Direction::right) {
            addNeighbourAboveLeft(row, num, Color::blue, neighbours, visited, emptyAllowed);
        }
    }

    void RedStraightNeighbourHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                          const NeighboursHelper::VisitedType *visited, bool emptyAllowed) const {
        const auto &row = cellCoords->row;
        const auto &num = cellCoords->num;
        addNeighbourRight(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourBelowRight(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourBelow(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourAbove(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourAboveLeft(row, num, Color::red, neighbours, visited, emptyAllowed);
        addNeighbourLeft(row, num, Color::red, neighbours, visited, emptyAllowed);
    }

    void BlueStraightNeighbourHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                           const NeighboursHelper::VisitedType *visited, bool emptyAllowed) const {
        const auto &row = cellCoords->row;
        const auto &num = cellCoords->num;
        addNeighbourBelow(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourBelowRight(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourRight(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourLeft(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourAboveLeft(row, num, Color::blue, neighbours, visited, emptyAllowed);
        addNeighbourAbove(row, num, Color::blue, neighbours, visited, emptyAllowed);

    }

    void EmptyNeighbourHelper::fill(List<CellCoords *> &neighbours, const CellCoords *cellCoords,
                                    const NeighboursHelper::VisitedType *visited, bool emptyAllowed) const {
        const auto &row = cellCoords->row;
        const auto &num = cellCoords->num;
        addNeighbourAboveLeft(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourAbove(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourRight(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourBelowRight(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourBelow(row, num, mColor, neighbours, visited, emptyAllowed);
        addNeighbourLeft(row, num, mColor, neighbours, visited, emptyAllowed);
    }

    void NeighboursHelper::addNeighbourBelow(size_t row, size_t num, Color color, List<CellCoords *> &neighbours,
                                             const VisitedType *visited, bool emptyAllowed) const {
        if (row < mBoard.size() - 1) {
            addToListIfNotVisited(row + 1, num, CellCoords::Direction::down, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourAbove(size_t row, size_t num, Color color, List<CellCoords *> &neighbours,
                                             const VisitedType *visited, bool emptyAllowed) const {
        if (row > 0) {
            addToListIfNotVisited(row - 1, num, CellCoords::Direction::up, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void
    NeighboursHelper::addNeighbourAboveLeft(size_t row, size_t num, Color color, List<CellCoords *> &neighbours,
                                            const VisitedType *visited, bool emptyAllowed) const {
        if (row > 0 && num > 0) {
            addToListIfNotVisited(row - 1, num - 1, CellCoords::Direction::upLeft, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void
    NeighboursHelper::addNeighbourBelowRight(size_t row, size_t num, Color color, List<CellCoords *> &neighbours,
                                             const VisitedType *visited, bool emptyAllowed) const {
        if (row < mBoard.size() - 1 && num < mBoard.size() - 1) {
            addToListIfNotVisited(row + 1, num + 1, CellCoords::Direction::downRight, row, num, neighbours, visited,
                                  color, emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourLeft(size_t row, size_t num, Color color, List<CellCoords *> &neighbours,
                                            const VisitedType *visited, bool emptyAllowed) const {
        if (num > 0) {
            addToListIfNotVisited(row, num - 1, CellCoords::Direction::left, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void NeighboursHelper::addNeighbourRight(size_t row, size_t num, Color color, List<CellCoords *> &neighbours,
                                             const VisitedType *visited, bool emptyAllowed) const {
        if (num < mBoard.size() - 1) {
            addToListIfNotVisited(row, num + 1, CellCoords::Direction::right, row, num, neighbours, visited, color,
                                  emptyAllowed);
        }
    }

    void NeighboursHelper::addToListIfNotVisited(
            size_t row, size_t num, CellCoords::Direction direction, size_t parentRow, size_t parentNum,
            List<CellCoords *> &list, const VisitedType *visited, Color color, bool emptyAllowed) const {
        if (!(*visited)[row][num]) {
            if (mBoard.getColor(row, num) == color || (emptyAllowed && mBoard.getColor(row, num) == Color::empty)) {
                list.pushBack(new CellCoords{row, num, direction});
            }
        }
    }

    void EmptyNeighbourHelper::addToListIfNotVisited(
            size_t row, size_t num, CellCoords::Direction direction, size_t parentRow, size_t parentNum,
            List<CellCoords *> &list, const VisitedType *visited, Color color,
            bool emptyAllowed) const {
        if (mDistanceUpdater.shouldVisit(row, num, parentRow, parentNum)) {
            list.pushBack(new CellCoords{row, num, direction});
            mDistanceUpdater.updateDistance(row, num, parentRow, parentNum);
        }
    }

    void DistanceUpdater::updateDistance(size_t row, size_t num, size_t parentRow, size_t parentNum) const {
        size_t distance = mBoard.getColor(row, num) == Color::empty ? mDistances[parentRow][parentNum] + 1
                                                                    : mDistances[parentRow][parentNum];
        if (distance < mDistances[row][num])
            mDistances[row][num] = distance;
    }

    bool DistanceUpdater::shouldVisit(size_t row, size_t num, size_t parentRow, size_t parentNum) const {
        if (mBoard.getColor(row, num) == Color::empty)
            return mDistances[parentRow][parentNum] + 1 < mDistances[row][num];
        if (mBoard.getColor(row, num) == mColor)
            return mDistances[parentRow][parentNum] < mDistances[row][num];
        return false;
    }
}
