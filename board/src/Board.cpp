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
    using algorithms::WinVerificationAlgorithm;

    Board::Board(size_t size) : mSize(size),
                                mRedCellsCount(0),
                                mBlueCellsCount(0),
                                mBoard(size),
                                mRedDoneVerifier(*this),
                                mBlueDoneVerifier(*this),
                                mRedNeighboursHelper(*this),
                                mBlueNeighboursHelper(*this),
                                mRedStraightNeighboursHelper(*this),
                                mBlueStraightNeighboursHelper(*this) {
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

    bool Board::isGameWonByRed(const DistancesKeeper &distancesKeeper) const {
        return isGameWonByRed(distancesKeeper.getDistancesToRightBorder());
    }

    bool Board::isGameWonByBlue(const DistancesKeeper &distancesKeeper) const {
        return isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder());
    }

    bool Board::isGameWonByRed(const Board::DistancesType &distancesToRightBorder) const {
        for (size_t row = 0; row < size(); ++row) {
            if (distancesToRightBorder[row][0] == 0)
                return true;
        }
        return false;
    }

    bool Board::isGameWonByBlue(const Board::DistancesType &distancesTpBottomBorder) const {
        for (size_t num = 0; num < size(); ++num) {
            if (distancesTpBottomBorder[0][num] == 0)
                return true;
        }
        return false;
    }

    bool Board::isGameWonByRed(const WinVerificationAlgorithm &algorithm, List<CellCoords> &path) const {
        List<CellCoords *> startCoordsList;
        for (size_t row = 0u; row < size(); ++row) {
            if (getType(row, 0) == Cell::Type::red)
                startCoordsList.pushBack(new CellCoords{row, 0, CellCoords::Direction::right});
        }
        return algorithm(startCoordsList, path);
    }

    bool Board::isGameWonByBlue(const WinVerificationAlgorithm &algorithm, List<CellCoords> &path) const {
        List<CellCoords *> startCoordsList;
        for (size_t num = 0u; num < size(); ++num) {
            if (getType(0, num) == Cell::Type::blue)
                startCoordsList.pushBack(new CellCoords{0, num, CellCoords::Direction::left});
        }
        return algorithm(startCoordsList, path);
    }

    bool Board::isBoardPossible() const {
        if (!isBoardCorrect())
            return false;
        {
            List<CellCoords> redPath;
            const Dfs dfs(*this, mRedNeighboursHelper, mRedDoneVerifier);
            if (isGameWonByRed(dfs, redPath)) {
                if (mBlueCellsCount == mRedCellsCount)
                    return false;
                const Bfs bfs(*this, mRedStraightNeighboursHelper, mRedDoneVerifier);
                return !isGameWonByRed(bfs, redPath);
            }
        }
        {
            List<CellCoords> bluePath;
            const Dfs dfs(*this, mBlueNeighboursHelper, mBlueDoneVerifier);
            if (isGameWonByBlue(dfs, bluePath)) {
                if (mBlueCellsCount < mRedCellsCount)
                    return false;
                const Bfs bfs(*this, mBlueStraightNeighboursHelper, mBlueDoneVerifier);
                return !isGameWonByBlue(bfs, bluePath);
            }
        }
        return true;
    }

    bool Board::canRedWinInNMovesWithNaive(size_t n, const DistancesKeeper &distancesKeeper) {
        if (isGameWonByRed(distancesKeeper.getDistancesToRightBorder()) ||
            isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder())) {
            return false;
        }
        bool movesFirst = mRedCellsCount == mBlueCellsCount;
        return canWinInNMovesWithNaive(n, movesFirst, distancesKeeper.getDistancesToLeftBorder(),
                                       distancesKeeper.getDistancesToRightBorder());
    }

    bool Board::canBlueWinInNMovesWithNaive(size_t n, const DistancesKeeper &distancesKeeper) {
        if (isGameWonByRed(distancesKeeper.getDistancesToRightBorder()) ||
            isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder())) {
            return false;
        }
        bool movesFirst = mRedCellsCount > mBlueCellsCount;
        return canWinInNMovesWithNaive(n, movesFirst, distancesKeeper.getDistancesToTopBorder(),
                                       distancesKeeper.getDistancesToBottomBorder());
    }

    bool Board::canWinInNMovesWithNaive(size_t n, bool movesFirst, const Board::DistancesType &distancesToFirstBorder,
                                        const Board::DistancesType &distancesToSecondBorder) {
        size_t neededEmptyCells = movesFirst ? 2 * n - 1 : 2 * n;
        if (neededEmptyCells > getColorCount(Cell::Type::empty) || !isBoardCorrect())
            return false;
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