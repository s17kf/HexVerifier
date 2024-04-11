//
// Created by stefan on 01/04/24.
//

#include <climits>
#include "Board.h"

#include "DoneVerifiers.h"
#include "NeighboursHelpers.h"
#include "Dfs.h"
#include "Bfs.h"
#include "MinMax.h"


using data_structures::Vector;
using data_structures::List;

namespace board {
    using algorithms::Dfs;
    using algorithms::Bfs;
    using algorithms::WinVerificationAlgorithm;
    using algorithms::MinMax;

    Board::Board(size_t size) :
            mSize(size),
            mRedCellsCount(0),
            mBlueCellsCount(0),
            mBoard(size),
            mRedDoneVerifier(*this),
            mBlueDoneVerifier(*this),
            mRedNeighboursHelper(*this),
            mBlueNeighboursHelper(*this),
            mRedStraightNeighboursHelper(*this),
            mBlueStraightNeighboursHelper(*this),
            mBfsForRed(*this, mRedStraightNeighboursHelper, mRedDoneVerifier),
            mBfsForBlue(*this, mBlueStraightNeighboursHelper, mBlueDoneVerifier) {
        for (auto &row: mBoard) {
            row.init(size);
        }
    }


    size_t Board::getColorCount(Color color) const {
        switch (color) {
            case Color::red:
                return mRedCellsCount;
            case Color::blue:
                return mBlueCellsCount;
            case Color::empty:
                return mSize * mSize - mRedCellsCount - mBlueCellsCount;
        }
        throw std::invalid_argument("Get color count - invalid color!");
    }


    bool Board::isBoardCorrect() const {
        auto redPawns = getColorCount(Color::red);
        auto bluePawns = getColorCount(Color::blue);
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

    bool Board::isGameWonByRed(const WinVerificationAlgorithm &algorithm) const {
        List<CellCoords> path;
        return isGameWonByRed(algorithm, path);
    }

    bool Board::isGameWonByBlue(const WinVerificationAlgorithm &algorithm) const {
        List<CellCoords> path;
        return isGameWonByBlue(algorithm, path);
    }

    bool Board::isGameWonByRed(const WinVerificationAlgorithm &algorithm, List <CellCoords> &path) const {
        List<CellCoords *> startCoordsList;
        for (size_t row = 0u; row < size(); ++row) {
            if (getColor(row, 0) == Color::red)
                startCoordsList.pushBack(new CellCoords{row, 0, CellCoords::Direction::right});
        }
        return algorithm(startCoordsList, path);
    }

    bool Board::isGameWonByBlue(const WinVerificationAlgorithm &algorithm, List <CellCoords> &path) const {
        List<CellCoords *> startCoordsList;
        for (size_t num = 0u; num < size(); ++num) {
            if (getColor(0, num) == Color::blue)
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
        if (neededEmptyCells > getColorCount(Color::empty) || !isBoardCorrect())
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

    bool Board::canRedWinInNMovesWithPerfect(size_t n, const DistancesKeeper &distancesKeeper) {
        if (isGameWonByRed(distancesKeeper.getDistancesToRightBorder()) ||
            isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder())) {
            return false;
        }
        bool movesFirst = mRedCellsCount == mBlueCellsCount;
        MinMax minMax(*this, distancesKeeper, mBfsForRed, mBfsForBlue);
        return canWinInNMovesWithPerfect(n, movesFirst, Color::red, Color::blue, minMax,
                                         distancesKeeper.getDistancesToLeftBorder(),
                                         distancesKeeper.getDistancesToRightBorder(),
                                         distancesKeeper.getDistancesToTopBorder(),
                                         distancesKeeper.getDistancesToBottomBorder());
    }

    bool Board::canBlueWinInNMovesWithPerfect(size_t n, const DistancesKeeper &distancesKeeper) {
        if (isGameWonByRed(distancesKeeper.getDistancesToRightBorder()) ||
            isGameWonByBlue(distancesKeeper.getDistancesToBottomBorder())) {
            return false;
        }
        bool movesFirst = mRedCellsCount > mBlueCellsCount;
        MinMax minMax(*this, distancesKeeper, mBfsForRed, mBfsForBlue);
        return canWinInNMovesWithPerfect(n, movesFirst, Color::blue, Color::red, minMax,
                                         distancesKeeper.getDistancesToTopBorder(),
                                         distancesKeeper.getDistancesToBottomBorder(),
                                         distancesKeeper.getDistancesToLeftBorder(),
                                         distancesKeeper.getDistancesToRightBorder());
    }

    bool Board::canWinInNMovesWithPerfect(size_t n, bool movesFirst, Color playerColor, Color opponentColor,
                                          algorithms::MinMax &minMax,
                                          const Board::DistancesType &playerDistances1,
                                          const Board::DistancesType &playerDistances2,
                                          const Board::DistancesType &opponentDistances1,
                                          const Board::DistancesType &opponentDistances2) {
        size_t neededSteps = movesFirst ? 2 * n - 1 : 2 * n;
        if (neededSteps > getColorCount(Color::empty) || !isBoardCorrect()) {
            return false;
        }
        List<CellCoords> playerPossibleMoves;
        fillEmptyCells(playerPossibleMoves, playerDistances1, playerDistances2, n);
        if (playerPossibleMoves.size() < neededSteps)
            return false;
        size_t opponentMoves = movesFirst ? n - 1 : n;
        List<CellCoords> opponentPossibleMoves;
        fillEmptyCells(opponentPossibleMoves, playerDistances1, playerDistances2, opponentDistances1,
                       opponentDistances2, n, opponentMoves);
        if (movesFirst) {
            return MinMax::WIN_VALUE == minMax.evaluate(
                    neededSteps, MinMax::PlayerType::max, playerColor, opponentColor,
                    playerPossibleMoves, opponentPossibleMoves);
        } else {
            return MinMax::WIN_VALUE == minMax.evaluate(
                    neededSteps, algorithms::MinMax::PlayerType::min, opponentColor, playerColor,
                    opponentPossibleMoves, playerPossibleMoves);
        }
    }

    void Board::setColor(size_t row, size_t num, board::Color color) {
        if (getColor(row, num) != Color::empty)
            decColorCount(getColor(row, num));
        if (color != Color::empty)
            incColorCount(color);

        mBoard[row][num] = color;
    }

    void Board::incColorCount(board::Color color) {
        switch (color) {
            case Color::red:
                ++mRedCellsCount;
                return;
            case Color::blue:
                ++mBlueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to increment count of not allowed type of cell!");
        }
    }

    void Board::decColorCount(board::Color color) {
        switch (color) {
            case Color::red:
                --mRedCellsCount;
                return;
            case Color::blue:
                --mBlueCellsCount;
                return;
            default:
                throw std::invalid_argument("Try to decrement count of not allowed type of cell!");
        }
    }

    void Board::fillEmptyCells(List<CellCoords *> &cellList) {
        for (size_t row = 0u; row < size(); ++row) {
            for (size_t num = 0u; num < size(); ++num) {
                if (mBoard[row][num] == Color::empty) {
                    cellList.pushBack(new CellCoords{row, num});
                }
            }
        }
    }

    void Board::fillEmptyCells(List <CellCoords> &cellList, const DistancesType &playerDistances1,
                               const DistancesType &playerDistances2, size_t maxDistance) {
        bool canAddIdleCell = maxDistance > 1;
        for (size_t row = 0u; row < size(); ++row) {
            for (size_t num = 0u; num < size(); ++num) {
                if (mBoard[row][num] == Color::empty) {
                    if (playerDistances1[row][num] <= maxDistance && playerDistances2[row][num] <= maxDistance) {
                        cellList.pushBack({row, num});
                        continue;
                    }
                    if (canAddIdleCell) {
                        cellList.pushBack({row, num});
                        canAddIdleCell = false;
                    }
                }
            }
        }
    }

    void Board::fillEmptyCells(List <CellCoords> &cellList,
                               const DistancesType &playerDistances1,
                               const DistancesType &playerDistances2,
                               const DistancesType &opponentDistances1,
                               const DistancesType &opponentDistances2,
                               size_t playerMaxDistance,
                               size_t opponentMaxDistance) {
        bool canAddIdleCell = opponentMaxDistance > 1;
        for (int row = size() - 1; row >= 0; --row) {
            for (int num = size() - 1; num >= 0; --num) {
                if (mBoard[row][num] == Color::empty) {
                    if (((playerDistances1[row][num] <= playerMaxDistance &&
                          playerDistances2[row][num] <= playerMaxDistance) ||
                         (opponentDistances1[row][num] <= opponentMaxDistance &&
                          opponentDistances2[row][num] <= opponentMaxDistance))) {
                        cellList.pushBack({static_cast<size_t>(row), static_cast<size_t>(num)});
                        continue;
                    }
                    if (canAddIdleCell) {
                        cellList.pushBack({static_cast<size_t>(row), static_cast<size_t>(num)});
                        canAddIdleCell = false;
                    }
                }
            }
        }
    }


} // board