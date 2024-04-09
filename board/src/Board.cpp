//
// Created by stefan on 01/04/24.
//

#include "Board.h"

#include "DoneVerifiers.h"
#include "NeighboursHelpers.h"
#include "Dfs.h"
#include "Bfs.h"


using data_structures::Vector;
using data_structures::List;

namespace board {
    using algorithms::Dfs;
    using algorithms::Bfs;

    Board::Board(size_t size) : mSize(size),
                                mRedCellsCount(0),
                                mBlueCellsCount(0),
                                mBoard(size),
                                mRedDoneVerifier(*this),
                                mBlueDoneVerifier(*this),
                                mRedNeighboursHelper(*this),
                                mBlueNeighboursHelper(*this) {
        for (auto &row: mBoard) {
            row.init(size);
        }
    }


    size_t Board::getColorCount(Cell::Type color) const {
        switch (color) {
            case Cell::Type::red:
                return mRedCellsCount;
            case Cell::Type::blue:
                return mBlueCellsCount;
            case Cell::Type::empty:
                return mSize * mSize - mRedCellsCount - mBlueCellsCount;
        }
        throw std::invalid_argument("Get color count - invalid color!");
    }


    bool Board::isBoardCorrect() const {
        auto redPawns = getColorCount(Cell::Type::red);
        auto bluePawns = getColorCount(Cell::Type::blue);
        if (redPawns < bluePawns || redPawns > bluePawns + 1) {
            return false;
        }
        return true;
    }

    bool Board::isGameWonByRed() const {
        List<CellCoords> path;
        return isGameWonByRed(path);
    }

    bool Board::isGameWonByBlue() const {
        List<CellCoords> path;
        return isGameWonByBlue(path);
    }

    bool Board::isGameWonByRed(List<CellCoords> &path) const {
        Dfs dfs(*this, mRedNeighboursHelper, mRedDoneVerifier);
        List<CellCoords *> startCoordsList;
        for (size_t row = 0u; row < size(); ++row) {
            if (getType(row, 0) == Cell::Type::red)
                startCoordsList.pushBack(new CellCoords{row, 0, CellCoords::Direction::right});
        }
        return dfs(startCoordsList, path);
    }

    bool Board::isGameWonByBlue(List<CellCoords> &path) const {
        Dfs dfs(*this, mBlueNeighboursHelper, mBlueDoneVerifier);
        List<CellCoords *> startCoordsList;
        for (size_t num = 0u; num < size(); ++num) {
            if (getType(0, num) == Cell::Type::blue)
                startCoordsList.pushBack(new CellCoords{0, num, CellCoords::Direction::left});
        }
        return dfs(startCoordsList, path);
    }

    bool Board::isBoardPossible() const {
        if (!isBoardCorrect())
            return false;
        List<CellCoords> redPath;
        if (isGameWonByRed(redPath)) {
            if (mBlueCellsCount == mRedCellsCount)
                return false;
            return !isGameWonByRed(redPath);
        }
        List<CellCoords> bluePath;
        if (isGameWonByBlue(bluePath)) {
            if (mBlueCellsCount < mRedCellsCount)
                return false;
            return !isGameWonByBlue(bluePath);
        }
        return true;
    }

    bool Board::canRedWinInNMovesWithNaive(size_t n) {
        bool movesFirst = mRedCellsCount == mBlueCellsCount;
        size_t neededEmptyCells = movesFirst ? 2 * n - 1 : 2 * n;
        if (neededEmptyCells > getColorCount(Cell::Type::empty))
            return false;
        if (!isBoardCorrect() || isGameWonByRed() || isGameWonByBlue())
            return false;
        DistancesType distancesToLeftBorder(size());
        DistancesType distancesToRightBorder(size());
        DistancesType distancesToTopBorder(size());
        DistancesType distancesToBottomBorder(size());
        Bfs bfs(*this);
        bfs.fillDistancesForEmptyCells(distancesToLeftBorder, distancesToRightBorder, distancesToTopBorder,
                                       distancesToBottomBorder);
        return canWinInNMovesWithNaive(n, distancesToLeftBorder, distancesToRightBorder);
    }

    bool Board::canBlueWinInNMovesWithNaive(size_t n) {
        bool movesFirst = mRedCellsCount > mBlueCellsCount;
        size_t neededEmptyCells = movesFirst ? 2 * n - 1 : 2 * n;
        if (neededEmptyCells > getColorCount(Cell::Type::empty))
            return false;
        if (!isBoardCorrect() || isGameWonByRed() || isGameWonByBlue())
            return false;
        DistancesType distancesToLeftBorder(size());
        DistancesType distancesToRightBorder(size());
        DistancesType distancesToTopBorder(size());
        DistancesType distancesToBottomBorder(size());
        Bfs bfs(*this);
        bfs.fillDistancesForEmptyCells(distancesToLeftBorder, distancesToRightBorder, distancesToTopBorder,
                                       distancesToBottomBorder);

        return canWinInNMovesWithNaive(n, distancesToTopBorder, distancesToBottomBorder);
    }

    bool Board::canWinInNMovesWithNaive(size_t n, Board::DistancesType &distancesToFirstBorder,
                                        Board::DistancesType &distancesToSecondBorder) {
        List<CellCoords *> emptyCells;
        fillEmptyCells(emptyCells);
        if (n == 1) {
            bool winningCellExist = false;
            while (!emptyCells.empty()) {
                auto *coords = emptyCells.popFront();
                auto &row = coords->row;
                auto &num = coords->num;
                if (distancesToFirstBorder[row][num] > 1 || distancesToSecondBorder[row][num] > 1) {
                    delete coords;
                    continue;
                }
                winningCellExist = true;
                delete coords;
                break;
            }
            while (!emptyCells.empty()) {
                delete emptyCells.popFront();
            }
            return winningCellExist;
        }
        bool twoMoveWinningCellExist = false;
        bool oneMoveWiningCellExist = false;
        bool idleMoveCellToFirstBorderExist = false;
        bool idleMoveCellToSecondBorderExist = false;
        while (!emptyCells.empty()) {
            auto *coords = emptyCells.popFront();
            auto &row = coords->row;
            auto &num = coords->num;
            if (distancesToFirstBorder[row][num] == 1 && distancesToSecondBorder[row][num] == 2) {
                twoMoveWinningCellExist = true;
                delete coords;
                break;
            }
            if (distancesToFirstBorder[row][num] == 1 && distancesToSecondBorder[row][num] == 1) {
                oneMoveWiningCellExist = true;
            }
            if (distancesToFirstBorder[row][num] > 1) {
                idleMoveCellToFirstBorderExist = true;
            }
            if (distancesToSecondBorder[row][num] > 1) {
                idleMoveCellToSecondBorderExist = true;
            }
            delete coords;
        }
        while (!emptyCells.empty()) {
            delete emptyCells.popFront();
        }
        return twoMoveWinningCellExist ||
               (oneMoveWiningCellExist && (idleMoveCellToFirstBorderExist || idleMoveCellToSecondBorderExist));
    }

    void Board::setType(size_t row, size_t num, CellType type) {
        if (getType(row, num) != Cell::Type::empty)
            decColorCount(getType(row, num));
        if (type != Cell::Type::empty)
            incColorCount(type);

        mBoard[row][num].setType(type);
    }

    void Board::incColorCount(Cell::Type color) {
        switch (color) {
            case Cell::Type::red:
                ++mRedCellsCount;
                return;
            case Cell::Type::blue:
                ++mBlueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to increment count of not allowed type of cell!");
        }

    }

    void Board::decColorCount(Cell::Type color) {
        switch (color) {
            case Cell::Type::red:
                --mRedCellsCount;
                return;
            case Cell::Type::blue:
                --mBlueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to decrement count of not allowed type of cell!");
        }
    }

    void Board::fillEmptyCells(List<CellCoords *> &cellList) {
        for (size_t row = 0u; row < size(); ++row) {
            for (size_t num = 0u; num < size(); ++num) {
                if (mBoard[row][num].getType() == Cell::Type::empty) {
                    cellList.pushBack(new CellCoords{row, num});
                }
            }
        }
    }


} // board